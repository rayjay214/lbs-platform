#ifndef REDIS_CMD_LINE
#define REDIS_CMD_LINE

#include <Redis.h>
#include <gmqdirect.h>
#include <string>

#define MAX_REDIS_ARGVLEN 2048

bool redisArgv2gmqLine(const Redis::redisArgv& req, GMQDirect::Line& line);
bool gmqLine2redisArgv(const GMQDirect::Line& line, Redis::redisArgv& req);

#endif
