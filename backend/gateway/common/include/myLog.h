#ifndef MYLOG_H
#define MYLOG_H

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

//const char * log_format(const char *fmt, ...); 
const char * log_format(const char *fmt, ...) __attribute__((format(printf,1,2)));
extern log4cxx::LoggerPtr logger;

#ifndef USE_GMLOGAPI

#define MYLOG_TRACE(logger, fmt, ...) LOG4CXX_TRACE(logger, log_format(fmt, ## __VA_ARGS__))                                                                                                 
#define MYLOG_DEBUG(logger, fmt, ...) LOG4CXX_DEBUG(logger, log_format(fmt, ## __VA_ARGS__))                                                                                                 
#define MYLOG_INFO(logger, fmt, ...) LOG4CXX_INFO(logger, log_format(fmt, ## __VA_ARGS__))                                                                                                   
#define MYLOG_WARN(logger, fmt, ...) LOG4CXX_WARN(logger, log_format(fmt, ## __VA_ARGS__))                                                                                                   
#define MYLOG_ERROR(logger, fmt, ...) LOG4CXX_ERROR(logger, log_format(fmt, ## __VA_ARGS__))                                                                                                 
#define MYLOG_FATAL(logger, fmt, ...) LOG4CXX_FATAL(logger, log_format(fmt, ## __VA_ARGS__))

#endif

#endif
