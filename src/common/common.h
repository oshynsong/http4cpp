/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_COMMON_COMMON_H
#define HTTP4CPP_COMMON_COMMON_H

#define BEGIN_NAMESPACE \
namespace http4cpp { \

#define END_NAMESPACE }
#define http4cpp_ns http4cpp

BEGIN_NAMESPACE

// Global common variables
extern std::string      g_object_meta_prefix;

// Log facility
enum log_level_t{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERR   = (1 << 1) - 1,
    LOG_LEVEL_WARN  = (1 << 2) - 1,
    LOG_LEVEL_INFO  = (1 << 3) - 1,
    LOG_LEVEL_DEBUG = (1 << 4) - 1
};
extern log_level_t      g_log_level;
extern bool             g_log_behind;

#define FATAL(fmt, ...) LogUtil::should_log(LOG_LEVEL_FATAL) ? \
    LogUtil::log_fatal("%s %s(%d)%s: " fmt, TimeUtil::now_utctime().c_str(),\
            __FILE__, __LINE__, __func__, __VA_ARGS__) :\
    LogUtil::log_none()

#define ERROR(fmt, ...) LogUtil::should_log(LOG_LEVEL_ERR) ? \
    LogUtil::log_err("%s %s(%d)%s: " fmt, TimeUtil::now_utctime().c_str(),\
            __FILE__, __LINE__, __func__, __VA_ARGS__) :\
    LogUtil::log_none()

#define WARN(fmt, ...) LogUtil::should_log(LOG_LEVEL_WARN) ? \
    LogUtil::log_warn("%s %s(%d)%s: " fmt, TimeUtil::now_utctime().c_str(),\
            __FILE__, __LINE__, __func__, __VA_ARGS__) :\
    LogUtil::log_none()

#define INFO(fmt, ...) LogUtil::should_log(LOG_LEVEL_INFO) ? \
    LogUtil::log_info("%s %s(%d)%s: " fmt, TimeUtil::now_utctime().c_str(),\
            __FILE__, __LINE__, __func__, __VA_ARGS__) :\
    LogUtil::log_none()

#define DEBUG(fmt, ...) LogUtil::should_log(LOG_LEVEL_DEBUG) ? \
    LogUtil::log_debug("%s %s(%d)%s: " fmt, TimeUtil::now_utctime().c_str(),\
            __FILE__, __LINE__, __func__, __VA_ARGS__) :\
    LogUtil::log_none()

// Time of clock type macro
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME         0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC        CLOCK_REALTIME
#endif
#ifndef CLOCK_MONOTONIC_COARSE
#define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC
#endif

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
