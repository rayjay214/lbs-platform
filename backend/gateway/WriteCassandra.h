#ifndef _WRITE_CASSANDRA_H_
#define _WRITE_CASSANDRA_H_

#include "cassandra.h"
#include "global.h"

using namespace std;

extern string g_cassandra_hosts;

class WriteCassandra
{
public:
    WriteCassandra()
    {
        _cluster = NULL;
        _session = cass_session_new();
        _close_future = NULL;
    };
    
    ~WriteCassandra()
    { 
        close();
    };

   void print_error(CassFuture* future);
   bool init();
   void close();

   bool create_cluster();
   CassError connect_session();
   CassError execute_query(const char* query);

   int process_records(SVec& data);
   int dispatch_record(const std::string &table, std::vector<SVec> &values);

   CassError insert_gps(vector<SVec>& values);
   
private:
    CassCluster* _cluster;
    CassSession* _session;
    CassFuture* _close_future;
};


#endif  /* _WRITE_CASSANDRA_H_ */

