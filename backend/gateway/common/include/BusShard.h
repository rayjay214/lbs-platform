#ifndef BUS_SHARD_H
#define BUS_SHARD_H
#include <boost/config.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iterator>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <pthread.h>

using boost::multi_index_container;
using namespace boost::multi_index;
namespace bip=boost::interprocess;

namespace BUS_SHARD
{

#define SHARD_CACHE_KEY 0x11239
#define SHARD_CURRENT_KEY 0x11240

#define BUS_SHARD_MASTER_0_SHM	"busShard.master.client_0.shm"
#define BUS_SHARD_SUBLINE_0_SHM	"busShard.subline.client_0.shm"
#define BUS_SHARD_USER_0_SHM	"busShard.user.client_0.shm"
#define BUS_SHARD_EID_0_SHM		"busShard.eid.client_0.shm"
#define BUS_SHARD_CITY_0_SHM	"busShard.city.client_0.shm"

#define BUS_SHARD_MASTER_0_INDEX	"busShard.master.client0.index"
#define BUS_SHARD_SUBLINE_0_INDEX	"busShard.subline.client0.index"
#define BUS_SHARD_USER_0_INDEX		"busShard.user.client0.index"
#define BUS_SHARD_EID_0_INDEX		"busShard.eid.client0.index"
#define BUS_SHARD_CITY_0_INDEX		"busShard.city.client0.index"


#define BUS_SHARD_MASTER_1_SHM	"busShard.master.client_1.shm"
#define BUS_SHARD_SUBLINE_1_SHM	"busShard.subline.client_1.shm"
#define BUS_SHARD_USER_1_SHM	"busShard.user.client_1.shm"
#define BUS_SHARD_EID_1_SHM		"busShard.eid.client_1.shm"
#define BUS_SHARD_CITY_1_SHM	"busShard.city.client_1.shm"


#define BUS_SHARD_MASTER_1_INDEX	"busShard.master.client1.index"
#define BUS_SHARD_SUBLINE_1_INDEX	"busShard.subline.client1.index"
#define BUS_SHARD_USER_1_INDEX		"busShard.user.client1.index"
#define BUS_SHARD_EID_1_INDEX		"busShard.eid.client1.index"
#define BUS_SHARD_CITY_1_INDEX		"busShard.city.client1.index"


///////////////////////////////////////////////////////////////////////////////////
struct userSharding
{
    uint64_t user_id;
    uint64_t citycode;
    userSharding()
    {
        user_id = citycode = 0;
    }
};

struct modify_udata
{
    modify_udata(const userSharding &data):v(data) {}

    void operator()(userSharding& data)
    {
        data = v;
    }
private:
    userSharding v;
};

typedef multi_index_container<
userSharding,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(userSharding,uint64_t,user_id)>
>,
bip::allocator<userSharding,bip::managed_shared_memory::segment_manager>
> user_con;
typedef boost::multi_index::nth_index<user_con,0>::type userIdx;

///////////////////////////////////////////////////////////////////////////////////
struct masterSharding
{
    uint32_t masterid;
    uint64_t citycode;
    masterSharding()
    {
        masterid = citycode = 0;
    }
};
struct modify_mdata
{
    modify_mdata(const masterSharding &data):v(data) {}

    void operator()(masterSharding& data)
    {
        data = v;
    }
private:
    masterSharding v;
};

typedef multi_index_container<
masterSharding,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(masterSharding,uint32_t,masterid)>
>,
bip::allocator<masterSharding,bip::managed_shared_memory::segment_manager>
> master_con;
typedef boost::multi_index::nth_index<master_con,0>::type masterIdx;

///////////////////////////////////////////////////////////////////////////////////
struct EidSharding
{
    uint64_t eid;
    uint64_t citycode;
    EidSharding()
    {
        eid = citycode = 0;
    }
};
struct modify_edata
{
    modify_edata(const EidSharding &data):v(data) {}

    void operator()(EidSharding& data)
    {
        data = v;
    }
private:
    EidSharding v;
};

typedef multi_index_container<
EidSharding,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(EidSharding,uint64_t,eid)>
>,
bip::allocator<EidSharding,bip::managed_shared_memory::segment_manager>
> eid_con;
typedef boost::multi_index::nth_index<eid_con,0>::type eidIdx;

///////////////////////////////////////////////////////////////////////////////////
struct sublineSharding
{
    uint32_t sublineid;
    uint64_t citycode;
    sublineSharding()
    {
        sublineid = citycode = 0;
    }
};
struct modify_sdata
{
    modify_sdata(const sublineSharding &data):v(data) {}

    void operator()(sublineSharding& data)
    {
        data = v;
    }
private:
    sublineSharding v;
};

typedef multi_index_container<
sublineSharding,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(sublineSharding,uint32_t,sublineid)>
>,
bip::allocator<sublineSharding,bip::managed_shared_memory::segment_manager>
> subline_con;
typedef boost::multi_index::nth_index<subline_con,0>::type sublineIdx;
///////////////////////////////////////////////////////////////////////////////////
typedef bip::managed_shared_memory::allocator<char>::type	char_allocator;
typedef bip::basic_string<char, std::char_traits<char>, char_allocator> shm_string;

struct citySharding
{
    uint64_t citycode;
    int isopen;
    int shardid;
    time_t moditime;
    shm_string cityname;
    shm_string domain;
    shm_string metrourl;
    shm_string qq_qun;
    shm_string support;
    citySharding(bip::managed_shared_memory* seg = NULL);
    citySharding& operator= (const citySharding& rhs)
    {
        citycode = rhs.citycode;
        isopen   = rhs.isopen;
        shardid  = rhs.shardid;
        cityname = rhs.cityname;
        domain   = rhs.domain;
        moditime = rhs.moditime;
        metrourl = rhs.metrourl;
        qq_qun   = rhs.qq_qun;
        support  = rhs.support;
        return *this;
    }
};

struct modify_cdata
{
    modify_cdata(const citySharding &data):v(data) {}

    void operator()(citySharding& data)
    {
        data = v;
    }
private:
    citySharding v;
};

typedef multi_index_container<
citySharding,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(citySharding,uint64_t,citycode)>
//ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(citySharding,int,isopen)>
>,
bip::allocator<citySharding,bip::managed_shared_memory::segment_manager>
> city_con;
typedef boost::multi_index::nth_index<city_con,0>::type cityIdx;
//typedef boost::multi_index::nth_index<city_con,1>::type cityIsOpenIdx;

///////////////////////////////////////////////////////////////////////////////////


class CBusShard
{
public:
    CBusShard();
    ~CBusShard();
    int init(int maxLineCount, int maxUserCount, int maxCityCount,
                    const char* pMasterShmName,const char* pMasterIndexName,
                    const char* pEidShmName,const char* pEidIndexName,
                    const char *pSublineShmName,const char *pSublineIndexName,
                    const char *pUserShmName,const char *pUserIndexName,
                    const char *pCityShmName,const char *pCityIndexName);

    int open(const char* pMasterShmName,const char* pMasterIndexName,
                    const char* pEidShmName,const char* pEidIndexName,
                    const char *pSublineShmName,const char *pSublineIndexName,
                    const char *pUserShmName,const char *pUserIndexName,
                    const char *pCityShmName,const char *pCityIndexName);


	int getUser(uint64_t userid);
	int getMaster(uint32_t masterid);
	int getEid(uint64_t eid);
	int getSubline(uint32_t sublineid);
	int getCity(uint64_t citycode);
    int setCity(citySharding &data);
    int setUser(userSharding &data);
    int setMaster(masterSharding &data);
    int setSubline(sublineSharding &data);
    int setEid(EidSharding &data);

	int delUser(userSharding &data);
    int delMaster(masterSharding &data);
    int delSubline(sublineSharding &data);
    int delEid(EidSharding &data);


    user_con	*u_con;
    master_con *m_con;
    subline_con *s_con;
    eid_con *e_con;
	city_con *c_con;

    bip::managed_shared_memory m_seg;
    bip::managed_shared_memory s_seg;
    bip::managed_shared_memory u_seg;
    bip::managed_shared_memory e_seg;
	bip::managed_shared_memory c_seg;

    uint32_t m_maxLineCount;
    uint32_t m_maxUserCount;
	uint32_t m_maxCityCount;

    std::string m_MasterShmName;
    std::string m_MasterIndexName;
    std::string m_EidShmName;
    std::string m_EidIndexName;
    std::string m_SublineShmName;
    std::string m_SublineIndexName;
    std::string m_UserShmName;
    std::string m_UserIndexName;
    std::string m_CityShmName;
    std::string m_CityIndexName;
};

typedef struct
{
    uint64_t citycode;
    int isopen;
    int shardid;
    time_t moditime;
    std::string cityname;
    std::string domain;
    std::string metro_url;
    std::string qq_qun;
    std::string support;
}CITY;

int32_t getCityInfoByCitycode(int64_t citycode, CITY& cityInfo);
int32_t getBusShardByCitycode(int64_t citycode);
int32_t getBusShardByMasterID(uint32_t masterid);
int32_t getBusShardBySublineID(uint32_t sublineid);
int32_t getBusShardByEID(uint64_t eid);
int32_t getBusShardByUserid(uint64_t userid);

int32_t getOpenCity(std::vector<CITY> &data);
int32_t getAllShard(std::set<int> &data);


}
#endif // IPLIMIT_H
