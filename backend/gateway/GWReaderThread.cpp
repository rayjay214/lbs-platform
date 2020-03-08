/*
 * threads.cpp
 *
 *  Created on: Jul 29, 2011
 *      Author: jim
 */

#include <errno.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "restart.h"
#include "rdwrn.h"
#include "inet_sockets.h"
#include "tostring.h"
#include "StringUtility.h"
#include "TimeTransformer.h"

#include "ggwapp.h"
#include "t808Parser.h"
#include "GWReaderThread.h"



ID_TAB _idTable(MAX_CONN_COUNT);
CONN_BUFFER g_ConnBuffer[MAX_CONN_COUNT];


GWReader::GWReader(int id, std::vector<int>& listenfds): _Id(id)
{
    _numOpenFds=0;
    _isStop = false;
    std::vector<int>::iterator it;
    for(it=listenfds.begin();it!=listenfds.end();it++)
    {
        _listenfds.insert(*it);
    }

	m_purgeCount = 0;
	m_connCount = 0;
	m_failMsgCount = 0;
}

GWReader::~GWReader()
{
}

void GWReader::run()
{
    int ready;
    int errcode;
    struct epoll_event ev;
    //void * pv;
    if (_listenfds.empty())
    {
        MYLOG_ERROR(g_logger, "Not specify listen ports");
        exit(EXIT_FAILURE);
    }

    // Applies events space
    if ((_epfd = epoll_create(MAX_READ_EVENTS)) == -1)
    {
        MYLOG_ERROR(g_logger, "%s:gw: Error epoll_create: %s", _threadName.c_str(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    std::set<int>::iterator it;
    for(it=_listenfds.begin();it!=_listenfds.end();it++)
    {
        ev.events = EPOLLIN;
        ev.data.fd = *it;
        if (epoll_ctl(_epfd, EPOLL_CTL_ADD, *it, &ev) == -1)
        {
            MYLOG_ERROR(g_logger, "%s:gw: Error epoll_ctl listenfd: %s", _threadName.c_str(), strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    MYLOG_INFO(g_logger, "Start %s", _threadName.c_str());
    time_t newt, gps_oldt;
    newt = time(NULL);

    while (!_isStop)
    {
        ready = epoll_wait(_epfd, _evlist, MAX_READ_EVENTS, EPOLL_TIMEOUT_MS);
        if (ready == -1)
        {
            if (errno == EINTR)
                continue;
            else
            {
                MYLOG_ERROR(g_logger, "%s gwd: Error epoll_wait: %s", _threadName.c_str(), strerror(errno));
                continue;
            }
        }

		//发生超时
        if (ready == 0)
        {
        	continue;
        }

        /* Deal with returned list of events */
        int j;
        for (j = 0; j < ready; ++j)
        {
            if (_evlist[j].events & EPOLLIN)
            {
                if (_listenfds.find(_evlist[j].data.fd) != _listenfds.end())
                {
                    int len = sizeof(struct sockaddr);
                    struct sockaddr_in netclient;
                    int cfd;
                    /* New client socket */
                    while (((cfd =
                            accept(_evlist[j].data.fd, (struct sockaddr *)(&netclient), (socklen_t*)&len)) == -1) &&
                           (errno == EINTR));

                    if(cfd == -1)
                    {
                        if(errno != 11)
                            MYLOG_ERROR(g_logger, "%s:gw: Error u_accept: %s", _threadName.c_str(), strerror(errno));
                        continue;
                    }
                    uint16_t port = ntohs(netclient.sin_port);
                    MYLOG_DEBUG(g_logger, "GWReader::run: u_accept  host:[%s:%d],fd=%d ", inet_ntoa(netclient.sin_addr),port,cfd);
                    if (keepalive(cfd, _expir) == -1)
                        MYLOG_ERROR(g_logger, "%s:gw: setsockopt keepalive error", _threadName.c_str());

                    if (set_nonblock(cfd) == -1)
                        MYLOG_ERROR(g_logger, "%s:gw: fcntl set_nonblock error", _threadName.c_str());

                    ev.events = EPOLLIN;
                    ev.data.fd = cfd;
                    if (epoll_ctl(_epfd, EPOLL_CTL_ADD, cfd, &ev) == -1)
                    {
                        MYLOG_ERROR(g_logger, "%s:gw: Error epoll_ctl EPOLL_CTL_ADD: %s", _threadName.c_str(), strerror(errno));
                        r_close(cfd);
                        continue;
                    }

                    ++_numOpenFds;
                    if(_numOpenFds >= MAX_CONN_COUNT)
                    {
                        MYLOG_ERROR(g_logger,"overload cfd=%d",cfd);
                        r_close(cfd);
                        continue;
                    }
                    g_ConnBuffer[cfd].used = 1;
                    g_ConnBuffer[cfd].tail = 8;
                    g_ConnBuffer[cfd].id = 0;
                    g_ConnBuffer[cfd].access_time = time(NULL);
                    g_ConnBuffer[cfd].ip = netclient.sin_addr.s_addr;
                    g_ConnBuffer[cfd].readerId = _Id;
                }
                else
                {
                    errcode = handleRequest(_evlist[j].data.fd);
                    if (errcode != 0)
                    {
                        MYLOG_WARN(g_logger, "%s:gw: handleRequest errcode=%d, cfd=%d",
                            _threadName.c_str(), errcode, _evlist[j].data.fd);
                        if (! purgeFd(_evlist[j].data.fd, true,errcode))
                        {
                            MYLOG_WARN(g_logger, "%s: gw: Failed to close connection cfd = %d",
                                _threadName.c_str(), _evlist[j].data.fd);
                        }
                    }
                }
            }
            else if (_evlist[j].events & (EPOLLHUP | EPOLLERR))
            {
                MYLOG_INFO(g_logger, "%s:gw: received EPOLLHUP | EPOLLERR cfd=%d",
                    _threadName.c_str(), _evlist[j].data.fd);
                if (! purgeFd(_evlist[j].data.fd, true,88))
                    MYLOG_WARN(g_logger, "%s:gw: Failed to close connection cfd=%d", _threadName.c_str(), _evlist[j].data.fd);
            }
        }
    }

    // For clean exit
    MYLOG_INFO(g_logger, "Stop %s", _threadName.c_str());
	gps_oldt = time(NULL);
	MYLOG_INFO(g_logger, "Time passed %ld", gps_oldt-newt);
}


int GWReader::handleRequest(int fd)
{
	int ret = 0;

    CONN_BUFFER* pConnBuffer = &g_ConnBuffer[fd];
    if(pConnBuffer->used == 0)
    {
        MYLOG_WARN(g_logger, "%s %s: connection not found! cfd=%d",
            _threadName.c_str(), __func__, fd);
        return -1;
    }
    if(pConnBuffer->tail >= CONN_BUFFER_LENGTH)
    {
        MYLOG_FATAL(g_logger, "%s %s: buffer is full! max=%d, tail=%d, fd=%d",
            _threadName.c_str(), __func__, CONN_BUFFER_LENGTH, pConnBuffer->tail, fd);
        return -2;
    }
    ssize_t read_size = read(fd, pConnBuffer->buffer + pConnBuffer->tail,CONN_BUFFER_LENGTH - pConnBuffer->tail);
    if(read_size == 0)
    {
        MYLOG_WARN(g_logger, "%s %s: connection close. cfd=%d",
            _threadName.c_str(), __func__, fd);
        return -10000;
    }
    else if(read_size == -1)
    {
        MYLOG_WARN(g_logger, "%s %s: socket read error! cfd=%d, errno=%d",
            _threadName.c_str(), __func__, fd, errno);
        return -10001;
    }
    else
    {
        pConnBuffer->tail += read_size;
        uint8_t stx = *(uint8_t*)(&pConnBuffer->buffer[8]);
        uint16_t stx_2 = 0;
        if (read_size >= 2)
            stx_2 = *(uint16_t*)(&pConnBuffer->buffer[8]);
        switch (stx)
        {
        case 0x7e:
            ret = handleT808Msg(fd);
            if (ret!=0)
            {
                MYLOG_WARN(g_logger, "%s %s: handleT808Msg error! cfd=%d, ret=%d",
                    _threadName.c_str(), __func__, fd, ret);
            }
            break;
        default:
            {
                MYLOG_WARN(g_logger, "%s %s: SYNC_START_NOT_MATCH! cfd=%d, stx=%x, stx_2=%x",
                    _threadName.c_str(), __func__, fd, stx, stx_2);
                ret = -3;
            }
        }
    }

    return ret;
}

int GWReader::handleT808Msg(int fd)
{
    CONN_BUFFER* pConnBuffer = &g_ConnBuffer[fd];
    if(pConnBuffer->used == 0)
    {
        MYLOG_WARN(g_logger, "%s %s: connection not found cfd = %d", _threadName.c_str(), __func__, fd);
        return -1;
    }

	std::string strIp = GetStrIp(g_ConnBuffer[fd].ip);
    do{
        uint8_t* pCur = pConnBuffer->buffer + GPP_IMEI_LEN;
        uint32_t packageSize = pConnBuffer->tail - GPP_IMEI_LEN;
        if(packageSize < 3)
        {
            //continue read
            if(packageSize > 0)
            {
                MYLOG_DEBUG(g_logger,"%s %s: continue read, current fd=%d, size=%d, packageSize=%d",
                    _threadName.c_str(), __func__,fd, pConnBuffer->tail, packageSize);
            }
            break;
        }

        DebugBinaryStr((const char*)pCur, packageSize);

        uint32_t otherPacketSize = 0;
        //只转义第一条数据，防止多次转义  如 7d010101 转义为 7d0101 后 如果再次转义就会变成 7d01
        int ret = T808Parser::Transform0x7e(pCur, (uint32_t*)&packageSize, (uint32_t*)&otherPacketSize);
        if (ret == TRANSFORM0X7E_ERROR)
        {
            //设备上传了错误的数据包，无法解析
            MYLOG_WARN(g_logger,"transform0x7e failed! current fd=%d, size=%d, packageSize=%d",
                fd, pConnBuffer->tail, packageSize);
            return ret;
        }
        else if(ret == PACKET_INCOMPLETE)
        {
            //没有转义一个完整的包，说明包被截断了
            if (pConnBuffer->tail >= CONN_BUFFER_LENGTH)
            {
                //buffer已经满了，那表示包的大小超过了buffer的size
                MYLOG_WARN(g_logger, "The package size is bigger than the size of buffer! current fd=%d, size=%d, packageSize=%d",
                    fd, pConnBuffer->tail, packageSize);
                return -101;
            }
            else
            {
                //buffer没满，继续读socket
                break;
            }
        }

        MYLOG_DEBUG(g_logger, "Transform0x7e success. current fd=%d, size=%d, packageSize=%u, otherPacketSize=%u",
                    fd, pConnBuffer->tail, packageSize, otherPacketSize);

        T808Parser oParser;
        std::string str((char*)pCur, packageSize);
        if (0 != oParser.HeaderParser(str))
        {
            MYLOG_WARN(g_logger, "HeaderParser error! current fd=%d, size=%d, packageSize=%d",
                    fd, pConnBuffer->tail, packageSize);
            return -102;
        }

        if (g_ConnBuffer[fd].id == 0)
        {
            std::string strImei = oParser.GetImei();
            uint64_t devId = 0;
            if (GetRedisDevId(strImei, devId) && devId != 0)
            {
                g_ConnBuffer[fd].id = devId;
                MYLOG_DEBUG(g_logger, "SetDevId fd=%d, devId=%lu, strImei=%s, ",
                    fd, g_ConnBuffer[fd].id, strImei.c_str());
            }
            else
            {
                MYLOG_DEBUG(g_logger, "GetRedisDevId failed! strImei=%s", strImei.c_str());
                return -2;
            }

            insertIdTable(devId, fd);
        }

        oParser.SetDevId(g_ConnBuffer[fd].id);
        MYLOG_DEBUG(g_logger, "SetDevId fd=%d, devId=%lu", fd, g_ConnBuffer[fd].id);

        uint16_t uiPackSize = oParser.GetMsgLen();
        if(uiPackSize > packageSize)
        {
            //continue read
            MYLOG_DEBUG(g_logger,"%s %s: continue read, current fd=%d, devid=%lu, size=%d, packageSize=%d, uiPackSize=%d",
                        _threadName.c_str(),  __func__,fd, pConnBuffer->id, pConnBuffer->tail, packageSize, uiPackSize);
            break;
        }

        std::string strRsp;
        std::string package((char*)pCur, uiPackSize);
        if (0 == oParser.MsgParser(package, strRsp, false))
        {
            if (!strRsp.empty())
            {
                DebugBinaryStr(strRsp.data(), strRsp.length());
                if ((unsigned)writen(fd, strRsp.data(), strRsp.length()) != strRsp.length())
                {
                    return -3;
                }
            }

            //如果是鉴权消息 需要下发断电指令
            if (oParser.GetMsgId() == T808_AUTH_MSGID)
            {
                std::string key = T_FOTA_BMS_INFO + ToString(oParser.GetDevId());
                std::string filed = "relay_status";
                std::string strBmsCtrlStatus = getRedisHashVal(key, filed);
                if (!strBmsCtrlStatus.empty())
                {
                    std::string strCmdReq;
                    if (strBmsCtrlStatus == "0")
                    {
                        strCmdReq = T808Parser::CommandReqMsg(oParser.GetImei(), BMS_RELAY_0);
                    }
                    else
                    {
                        strCmdReq = T808Parser::CommandReqMsg(oParser.GetImei(), BMS_RELAY_0);
                    }

                    std::string strHex = Goome::Binary2HexFormatString(strCmdReq.data(), strCmdReq.length());
                    MYLOG_DEBUG(g_logger, "send bms ctrl cmd. devid=%lu, imei=%s, relay_status=%s, cmd=%s",
                        oParser.GetDevId(), oParser.GetImei().c_str(), strBmsCtrlStatus.c_str(), strHex.c_str());
                    writen(fd, strCmdReq.data(), strCmdReq.length());
                }
            }
        }

        pConnBuffer->tail -= otherPacketSize;
        memmove(pConnBuffer->buffer+8,pConnBuffer->buffer+8+otherPacketSize,pConnBuffer->tail-8);
    }while(1);

    return 0;
}

std::string GWReader::GetStrIp(uint32_t ip)
{
	char IPdotdec[20] = {0};
	inet_ntop(AF_INET, (struct in_addr*)&ip,IPdotdec,16);
	return std::string(IPdotdec);
}

void GWReader::setThreadName(std::string threadName)
{
    _threadName = threadName;
}

void GWReader::stop()
{
    _isStop = true;
}

void GWReader::setExpir(int expir)
{
    _expir = expir;
}

void GWReader::setPurgeTime(long time)
{
    _purgeTime = time;
}

bool GWReader::purgeFd(int purge_cfd, bool force, int reason)
{
	m_purgeCount++;
    if(g_ConnBuffer[purge_cfd].used != 0)
    {
        int cfd = -1;
        uint64_t id = g_ConnBuffer[purge_cfd].id;
        time_t at = g_ConnBuffer[purge_cfd].access_time;
        if ((false == force) && (at >= time(NULL) - (long)_purgeTime))
        {
            return false;
        }

        if(id != 0)
        {
            ID_TAB::const_accessor tab_accessor2;
            if (_idTable.find(tab_accessor2, id))
            {
                cfd = tab_accessor2->second.cfd;
                tab_accessor2.release();
            }

            if(cfd == purge_cfd)
            {
                _idTable.erase(id);
                MYLOG_DEBUG(g_logger, "%s:  idTable remove id= %llx  cfd= %d ,reason=%d,client ip = %s", _threadName.c_str(),ntoh64(id),purge_cfd,reason,inet_ntoa(*(struct in_addr*)&(g_ConnBuffer[purge_cfd].ip)));
            }
        }
        g_ConnBuffer[purge_cfd].used = 0;
		*(uint64_t *)(g_ConnBuffer[purge_cfd].buffer) = 0;
    }

    epoll_ctl(_epfd, EPOLL_CTL_DEL, purge_cfd, NULL);
    r_close(purge_cfd);
    --_numOpenFds;
    return true;
}


void GWReader::insertIdTable(uint64_t id,int cfd)
{
    m_connCount++;

    bool ires;
    int32_t oldcfd;
    ID_TAB::accessor id_tab_accessor;
    ires=_idTable.insert(id_tab_accessor, id);
    if(false==ires)//already exist,delete the old item
    {
        oldcfd = id_tab_accessor->second.cfd;
    }

    id_tab_accessor->second.cfd = cfd;
    id_tab_accessor.release();
    if(false == ires)
    {
        if(oldcfd != cfd)
        {
            purgeFd(oldcfd,true,90);
        }
    }
    else
    {
        MYLOG_DEBUG(g_logger, "%s: idTable insert id= %lx, cfd= %d, client ip = %s",
            _threadName.c_str(), id, cfd, inet_ntoa(*(struct in_addr*)&(g_ConnBuffer[cfd].ip)));
    }

    return;
}

void GWReader::setAcceptEnable(bool bEnable)
{
    struct epoll_event ev;
    std::set<int>::iterator it;
    for(it=_listenfds.begin();it!=_listenfds.end();it++)
    {
        ev.events = bEnable?EPOLLIN:0;
        ev.data.fd = *it;
        if (epoll_ctl(_epfd, EPOLL_CTL_MOD, *it, &ev) == -1)
        {
            MYLOG_WARN(g_logger, "%s[%d]:gw: Error epoll_ctl listenfd: %s", _threadName.c_str(), _Id,strerror(errno));
        }
    }
}

void GWReader::GetConnCount(uint64_t& purgeCount, uint64_t& connCount, uint64_t& failMsgCount)
{
	purgeCount = m_purgeCount;
	connCount = m_connCount;
	failMsgCount = m_failMsgCount;
	return;
}


int GWReader::SendMsgToClient(uint64_t devid, const std::string &msg)
{
    ID_TAB::const_accessor id_tab_accessor;
    int sendfd = -1;
    if (_idTable.find(id_tab_accessor, devid))
    {
        sendfd = id_tab_accessor->second.cfd;
        id_tab_accessor.release();
        if (g_ConnBuffer[sendfd].used == 1)
        {
            if (g_ConnBuffer[sendfd].id != devid)
            {
                MYLOG_WARN(g_logger, "id not macth connection id[%lu] , req devid [%lu]",
                     g_ConnBuffer[sendfd].id, devid);
                return -1;
            }

            if ((unsigned)writen(sendfd, msg.data(), msg.length()) != msg.length())
            {
                MYLOG_WARN(g_logger, "send error! sendfd=%d, devid=%lu, length=%zu",
                    sendfd, devid, msg.length());
                return -2;
            }
        }
        else
        {
            id_tab_accessor.release();
            MYLOG_WARN(g_logger, "connection not used! sendfd=%d, devid=%lu, length=%zu",
                    sendfd, devid, msg.length());
            return -3;
        }
    }
    else
    {
        id_tab_accessor.release();
        MYLOG_WARN(g_logger, "connection not found! sendfd=%d, devid=%lu, length=%zu",
                sendfd, devid, msg.length());
        return -4;
    }

    return 0;
}


