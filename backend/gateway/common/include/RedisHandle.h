#ifndef CLASS_REDIS_HANDLE_H
#define CLASS_REDIS_HANDLE_H

#include <string>
#include "redisclient.h"    // TODO: (Ou)这个版本的redis client API不太可靠，有一些bug（比如zadd已经存在的字段时会抛异常），需要人工修改。考虑替换成hiredis等官方的API
#include <time.h>
#include "gns_api_raii.h"

class RedisConnectPool;

class CRedisClient
{

public:
    CRedisClient(RedisConnectPool* pool);
    virtual ~CRedisClient();
    bool ReConnectRedis();
    void disConnect();
    void checkIdle();

    bool Get(const std::string & key, std::string & value);
    bool Set(const std::string & key, const std::string & value);
    bool Expire(const std::string & key, unsigned int secs);
    bool MGet(const std::vector<std::string> & keys, std::vector<std::string> & out);
	bool Mset(const std::vector<std::string> &key, const std::vector<std::string> &value);
    bool SetNx(const std::string& key, const std::string& value, bool & is_exist);
    bool SetEx(const std::string& key, const std::string& value, unsigned int secs);
    bool Del(const std::string & key);
    bool MDel(const std::vector<std::string> & keys);
    bool Incr(const std::string &key,int &val);
    bool Incrby(const std::string &key,long &increment, int &result);
    bool Decr(const std::string &key,int &val);

    bool SAdd(const std::string & key, const std::string & element);
    long SMembers(const std::string & key, std::set<std::string> &out);
    bool SRem(const std::string & key, const std::string & element);

    bool HSet(const std::string & key, const std::string & field, const std::string & value);//hashes
    bool HMSet(const std::string & key, const std::vector<std::string> &fields, const std::vector<std::string> & values);
    bool HGet(const std::string & key, const std::string & fieid, std::string & value);
    bool HMGet(const std::string & key, const std::vector<std::string> &fields, std::vector<std::string> &out);
	bool HGetAll(const std::string & key, std::vector<std::pair<std::string, std::string> > &out);
    bool HDel(const std::string & key, const std::string & field);
    bool HDel(const std::string & key, const std::vector<std::string> & fields);
    long HIncrby(const std::string & key, const std::string & fieid, long increment);

    bool Lpush(const std::string & key, const std::string & value);
    bool Llen(const std::string & key, int & value);
    bool Lrange(const std::string & key, int start, int end, std::vector<std::string>& out);
	bool Ltrim(const std::string & key, int start, int end);
	bool Lpop(const std::string & key, std::string & value);

	bool ZAdd(const std::string &key,double score,const std::string &member);
	bool ZScore(const std::string &key,const std::string &member,double &score);
    bool ZIncrby(const std::string &key,const std::string &member, double increment, double &score);
    bool ZRange(std::string key, int start, int end, std::vector<std::string> & out);
	bool ZRangeByScore(const std::string & key, double min, double max, std::vector<std::string> & out, int offset = 0, int max_count = -1, int range_modification = 0);
    bool ZRangeByScore(const std::string & key, double min, double max, std::vector< std::pair<std::string, double> > & out, int offset = 0, int max_count = -1, int range_modification = 0);
    bool ZRevRangeByScore(const std::string & key, double max, double min, std::vector<std::string> & out, int offset, int max_count, int range_modification);
    bool ZRevRangeByScore(const std::string & key, double max, double min, std::vector< std::pair<std::string, double> > & out, int offset = 0, int max_count = -1, int range_modification = 0);
    bool ZCard(std::string key, int& num);
    bool ZCount(const std::string &key,double min,double max,int &num,int range_modification);
	bool ZRem(const std::string &key,const std::string &member);
    bool ZRemRangeByRank(const std::string &key, int start, int end);
    bool ZRemRangeByScore(const std::string &key,double min,double max);
    bool SISMEMBER(std::string key,std::string value);
    // Function: 获取key对应的member的rank。
    // Output: rank  当key和member存在时，rank>=0，当key和member不存在时，rank<0
    // Return: 操作是否成功
	bool ZRank(const std::string &key,const std::string &member,int &rank);
    bool ZRevRank(const std::string &key,const std::string &member,long &rank);
    bool ZRevRange(const std::string & key, long start, long end, std::vector< std::string > & out);

public:
	RedisConnectPool *m_redisPool;
	
private:
    redis::client*	m_predisClient;
    time_t m_tLast;

    GNS_API::CALLER m_oCaller;
    GNS_API::CALLEE m_oCallee;
};

#endif



