#include "tostring.h"
#include "StringUtility.h"
#include "TimeTransformer.h"
#include "dev.pb.h"
#include "WriteCassandra.h"

void WriteCassandra::print_error(CassFuture* future) 
{
  const char* message;
  size_t message_length;
  cass_future_error_message(future, &message, &message_length);
  MYLOG_ERROR(g_logger, "Cassandra Error: %s", message);
}


bool WriteCassandra::init()
{   
    if (!create_cluster())
    {
        return false;
    }

    if (CASS_OK != connect_session()) 
    {
        cass_cluster_free(_cluster);
        cass_session_free(_session);
        return false;
    }

    return true;
}

void WriteCassandra::close()
{
    _close_future = cass_session_close(_session);
    cass_future_wait(_close_future);
    cass_future_free(_close_future);

    cass_cluster_free(_cluster);
    cass_session_free(_session);
}

bool WriteCassandra::create_cluster()
{
    _cluster = cass_cluster_new();
    cass_cluster_set_contact_points(_cluster, g_cassandra_hosts.c_str());

    return true;
}

CassError WriteCassandra::connect_session() 
{
  CassError rc = CASS_OK;
  CassFuture* future = cass_session_connect(_session, _cluster);

  cass_future_wait(future);
  rc = cass_future_error_code(future);
  if (rc != CASS_OK) 
  {
      print_error(future);
  }

  cass_future_free(future);

  return rc;
}

CassError WriteCassandra::execute_query(const char* query) 
{
  CassError rc = CASS_OK;
  CassFuture* future = NULL;
  CassStatement* statement = cass_statement_new(query, 0);

  future = cass_session_execute(_session, statement);
  cass_future_wait(future);

  rc = cass_future_error_code(future);
  if (rc != CASS_OK) 
  {
      print_error(future);
  }

  cass_future_free(future);
  cass_statement_free(statement);

  return rc;
}

int WriteCassandra::process_records(SVec& data)
{
    std::string table;
    SVec vecVal;
    std::map<std::string, std::vector<SVec> > datamap;
    for(SVec::iterator it = data.begin(); it != data.end(); it++)
    {
        const std::string &strRecord = *it;

        table.clear();
        vecVal.clear();
        
        YunJi::UpDevMsg oPbMsg;
        if (!oPbMsg.ParseFromString(strRecord))
        {
            MYLOG_WARN(g_logger, "ParseFromString failed!");
            continue;
        }

        switch(oPbMsg.msgtype())
        {
            case YunJi::kGpsMsg:
            {
                table = "gps";
                const YunJi::GpsMsg &oPbGps = oPbMsg.gps();
                vecVal.push_back(ToString(oPbGps.id()));
                vecVal.push_back(ToString(oPbGps.datetime()));
                vecVal.push_back(ToString(oPbGps.route()));
                vecVal.push_back(ToString(oPbGps.lat()));
                vecVal.push_back(ToString(oPbGps.lng()));
                vecVal.push_back(ToString(PROTOCOL_T808));
                vecVal.push_back(ToString(oPbGps.seq()));
                vecVal.push_back(ToString(oPbGps.speed()));
                vecVal.push_back(ToString(oPbGps.systime()));
                break;
            }
            default:
                break;
        }

        if (!table.empty() && !vecVal.empty())
        {
            datamap[table].push_back(vecVal);
        }
    }

    std::map<std::string, std::vector<SVec>>::iterator iter;
    for (iter = datamap.begin(); iter != datamap.end(); iter++ )
    {
        const std::string &table = iter->first;
        dispatch_record(table, (*iter).second);
    }
    return 0;
}

int WriteCassandra::dispatch_record(const std::string &table, std::vector<SVec > &values)
{
    if ("gps" == table)
    {
        insert_gps(values);
    }
    else
    {
        MYLOG_ERROR(g_logger, "dispatch failed, table: %s", table.c_str());
    }

    return 0;
}

CassError WriteCassandra::insert_gps(std::vector<SVec>& values)
{
    CassError rc = CASS_OK;

    /*
    dev_id bigint,
    report_time timestamp,
    direction int,
    latitude int,
    longitude int,
    protocol_no int,
    seq_no int,
    speed int,
    sys_time timestamp,
    */

    const char* query = "INSERT INTO gpsinfo.gps \
          (dev_id, report_time, direction, latitude, \
           longitude, protocol_no, seq_no, speed, sys_time) \
          VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    int key_size = 9;
    vector<SVec>::iterator it;
    for( it = values.begin(); it != values.end(); it++ )
    {
        SVec& value = *it;
        time_t timeval_t = 0;
        int value_size = static_cast<int>(value.size());
        if (key_size != value_size)
        {
            MYLOG_ERROR(g_logger, "value size not same as key size: %d, %d", key_size, value_size);
            continue;
        }

        CassStatement* statement = NULL;
        CassFuture* future = NULL;
        statement = cass_statement_new(query, key_size);

        cass_int64_t timeval_int64_t = 0;
        
        for (int idx = 0; idx < value_size; ++idx)
        {
            if (idx == 0)
            {
                int64_t lval= atoll(value[idx].c_str());
                cass_statement_bind_int64(statement, idx, lval);
            }
            else if (idx == 1 || idx == 8)
            {
                timeval_t = atol(value[idx].c_str());
                timeval_int64_t = (cass_int64_t)(timeval_t * 1000);
                cass_statement_bind_int64(statement, idx, timeval_int64_t);
            }
            else
            {
                int nval = atol(value[idx].c_str());
                cass_statement_bind_int32(statement, idx, nval);
            }
        }

        future = cass_session_execute(_session, statement);
        cass_future_wait(future);

        rc = cass_future_error_code(future);
        if (rc != CASS_OK) 
        {
            print_error(future);
        }

        cass_future_free(future);
        cass_statement_free(statement);
    }

    return rc;
}


