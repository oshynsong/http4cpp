/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_COMMON_UTIL_H
#define HTTP4CPP_COMMON_UTIL_H

#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h> /* for ISO C variable arguments*/

#include <string>
#include <vector>

#include "common/common.h"

BEGIN_NAMESPACE

class LogUtil {
public:
    static inline const char * level_to_string(int level);
    static inline int level_to_syslog(int level);
    static bool should_log(int level);

    static void log_none();
    static void log_fatal(const char * fmt, ...);
    static void log_err(const char * fmt, ...);
    static void log_warn(const char * fmt, ...);
    static void log_info(const char * fmt, ...);
    static void log_debug(const char * fmt, ...);

private:
    static void logging(int level, const char * fmt, va_list ap);
    const static int MAXLINE = 10240;
};

class TimeUtil {
public:
    static void init();
    static int64_t now_ms();
    static int64_t now_us();
    static time_t now();
    static struct tm now_tm();
    static std::string now_utctime();
    static std::string now_gmttime();
    static std::string timestamp_to_utctime(time_t);
    static std::string timestamp_to_gmttime(time_t);
    static int64_t utctime_to_timestamp(const std::string &);
    static int64_t gmttime_to_timestamp(const std::string &);
    static int64_t get_utc_offset();

    static const char * UTC_FORMAT;
    static const char * GMT_FORMAT;
    static const int UTC_FORMAT_LENGTH;

    class TimeUtilInitializer {
    public:
        TimeUtilInitializer()
        {
            TimeUtil::init();
        }
    };
private:
    static int64_t              _s_utc_offset;
    static TimeUtilInitializer  _s_initializer;
};

// Hmac encryption type, here just need sha1 and others can be added.
enum hmac_type_t {
    HMAC_TYPE_SHA1 = 1
};

class StringUtil {
public:
    static int trim(const std::string &src, std::string &output, const std::string &c=EMPTY_CHARS);
    static int ltrim(const std::string &src, std::string &output, const std::string &c=EMPTY_CHARS);
    static int rtrim(const std::string &src, std::string &output, const std::string &c=EMPTY_CHARS);
    static std::string trim(const std::string &src, const std::string &c=EMPTY_CHARS);
    static std::string ltrim(const std::string &src, const std::string &c=EMPTY_CHARS);
    static std::string rtrim(const std::string &src, const std::string &c=EMPTY_CHARS);

    static std::string num_to_string(int num);
    static uint64_t string_to_num(std::string &str, uint8_t base=10);
    static int split(const std::string &src, const std::string &delimiter,
            int max_items, std::vector<std::string> * result);

    static int lower(const std::string &src, std::string &output);
    static int upper(const std::string &src, std::string &output);
    static std::string lower(const std::string &src);
    static std::string upper(const std::string &src);

    static std::string hex(unsigned char c);
    static std::string hex(const std::string &src);

    static std::string base64_encode(const std::string &src);
    static std::string base64_decode(const std::string &src);
    static std::string url_encode(const std::string &src, bool=true);
    static std::string url_decode(const std::string &src);

    static int hmac(int type, const std::string &key, const std::string &data, std::string *result);
    static std::string sha256hex(const std::string &src, const std::string &sk);
    static int iconv(const std::string &, const std::string &, const std::string &, std::string *);

private:
    static bool is_hex_char(unsigned char c);
    static inline unsigned char base64_decode_char(const unsigned char);
    const static char * HEX_CHARS;
    const static char * EMPTY_CHARS;
    const static char * BASE64_CHARS;
};

// Return code facilies
enum ret_code_t {
    RET_OK = 0,
    RET_SERVICE_ERROR = 1000,
    RET_CLIENT_ERROR,
    RET_INIT_CURL_FAIL,
    RET_ILLEGAL_ARGUMENT,
    RET_FILE_INVALID,
    RET_ILLEGAL_OPERATION,
};
const char * stringfy_ret_code(int code);

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
