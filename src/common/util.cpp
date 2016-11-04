/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include <stdio.h>  /* for c-style IO */
#include <syslog.h> /* for syslog */
#include <stdarg.h> /* for ISO C variable arguments*/
#include <ctype.h>  /* for upper/lower */
#include <limits.h> /* for UCHAR_MAX */
#include <stdlib.h> /* for exit */

#include <algorithm>/* for std algorithms */
#include <sstream>  /* string stream */

#include "common/util.h"

BEGIN_NAMESPACE

const char * LogUtil::level_to_string(int level)
{
    switch (level) {
        case LOG_LEVEL_FATAL:
            return "FATAL";
        case LOG_LEVEL_ERR:
            return "ERROR";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        default:
            return "DEBUG";
    }
}

int LogUtil::level_to_syslog(int level)
{
    switch (level) {
        case LOG_LEVEL_FATAL:
            return LOG_CRIT;
        case LOG_LEVEL_ERR:
            return LOG_ERR;
        case LOG_LEVEL_WARN:
            return LOG_WARNING;
        case LOG_LEVEL_INFO:
            return LOG_INFO;
        case LOG_LEVEL_DEBUG:
            return LOG_DEBUG;
        default:
            return LOG_DEBUG;
    }
}

void LogUtil::logging(int level, const char * fmt, va_list ap)
{
    char buf[MAXLINE];
    int n = vsnprintf(buf, MAXLINE - 2, fmt, ap);
    if (n == -1) {
        return;
    }
    buf[n] = '\n';
    buf[n + 1] = '\0';
    if (g_log_behind) {
        syslog(level_to_syslog(level), "%s", buf);
    } else {
        fflush(stdout);
        fprintf(stderr, "[%s]%s", level_to_string(level), buf);
        fflush(stderr);
    }
}

bool LogUtil::should_log(int level)
{
    return level <= g_log_level;
}

void LogUtil::log_none()
{
    // Nothing to do
}

void LogUtil::log_fatal(const char * fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    logging(LOG_LEVEL_FATAL, fmt, ap);
    va_end(ap);
    exit(1);
}

void LogUtil::log_err(const char * fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    logging(LOG_LEVEL_ERR, fmt, ap);
    va_end(ap);
}

void LogUtil::log_warn(const char * fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    logging(LOG_LEVEL_WARN, fmt, ap);
    va_end(ap);
}

void LogUtil::log_info(const char * fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    logging(LOG_LEVEL_INFO, fmt, ap);
    va_end(ap);
}

void LogUtil::log_debug(const char * fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    logging(LOG_LEVEL_DEBUG, fmt, ap);
    va_end(ap);
}


// TimeUtil definition
const char * TimeUtil::UTC_FORMAT= "%Y-%m-%dT%H:%M:%SZ";
const int TimeUtil::UTC_FORMAT_LENGTH= 20;
const char * TimeUtil::GMT_FORMAT= "%a, %d %b %Y %H:%M:%S GMT";
int64_t TimeUtil::_s_utc_offset = 0;
TimeUtil::TimeUtilInitializer _s_initializer;

void TimeUtil::init()
{
    time_t now_time = time(NULL);
    struct tm tm_gmt;
    gmtime_r(&now_time, &tm_gmt);

    struct tm tm_local;
    localtime_r(&now_time, &tm_local);

    _s_utc_offset = static_cast<int64_t>(mktime(&tm_local) - mktime(&tm_gmt));
}

int64_t TimeUtil::now_ms()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

int64_t TimeUtil::now_us()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000 + now.tv_usec;
}

time_t TimeUtil::now()
{
    return time(NULL);
}

struct tm TimeUtil::now_tm()
{
    time_t now_time = now();
    struct tm tm_struct;
    gmtime_r(&now_time, &tm_struct);
    return tm_struct;
}

std::string TimeUtil::now_utctime()
{
    return timestamp_to_utctime(now());
}

std::string TimeUtil::now_gmttime()
{
    return timestamp_to_gmttime(now());
}

std::string TimeUtil::timestamp_to_utctime(time_t ts)
{
    struct tm tm_result;
    gmtime_r(&ts, &tm_result);

    char buf[128];
    int size = strftime(buf, 128, UTC_FORMAT, &tm_result); 

    std::string result(buf, size);
    return result;
}

std::string TimeUtil::timestamp_to_gmttime(time_t ts)
{
    struct tm tm_result;
    gmtime_r(&ts, &tm_result);

    char buf[128];
    int size = strftime(buf, 128, GMT_FORMAT, &tm_result);

    std::string result(buf, size);
    return result;
}

int64_t TimeUtil::utctime_to_timestamp(const std::string & utc)
{
    struct tm tm_result;
    if (strptime(utc.c_str(), UTC_FORMAT, &tm_result)
            != utc.c_str() + UTC_FORMAT_LENGTH) {
        return -1;
    }

    return static_cast<int64_t>(mktime(&tm_result));
}

int64_t TimeUtil::gmttime_to_timestamp(const std::string & gmt)
{
    struct tm tm_result;
    if (strptime(gmt.c_str(), GMT_FORMAT, &tm_result) == NULL) {
        return -1;
    }

    return static_cast<int64_t>(mktime(&tm_result));
}

int64_t TimeUtil::get_utc_offset()
{
    return _s_utc_offset;
}


//StringUtil definition
const char * StringUtil::HEX_CHARS = "0123456789ABCDEF";
const char * StringUtil::EMPTY_CHARS = " \r\n\t";
const char * StringUtil::BASE64_CHARS = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

bool StringUtil::is_hex_char(unsigned char c)
{
    if ('a' <= c && 'f' >= c) {
        c = c - 32;
    }
    if (('0' <= 9 && '9' >= c) || ('A' <= c && 'F' >= c)) {
        return true;
    }
    return false;
}

int StringUtil::trim(const std::string &src, std::string &output, const std::string &c)
{
    std::string tmp;
    if (ltrim(src, tmp, c) < 0) {
        return -1;
    }
    return rtrim(tmp, output, c);
}

int StringUtil::ltrim(const std::string &src, std::string &output, const std::string &c)
{
    output.clear();
    size_t start = src.find_first_not_of(c);
    if (start == std::string::npos) {
        return 0;
    }
    output.resize(src.size() - start);
    std::string::iterator res = std::copy(src.begin() + start, src.end(), output.begin());
    if (res == output.end()) {
        return 0;
    }
    return -1;
}

int StringUtil::rtrim(const std::string &src, std::string &output, const std::string &c)
{
    output.clear();
    size_t end = src.find_last_not_of(c);
    if (end == std::string::npos) {
        return 0;
    }
    output.resize(end + 1);
    std::string::iterator res = std::copy(src.begin(), src.begin() + end + 1, output.begin());
    if (res == output.end()) {
        return 0;
    }
    return -1;
}

std::string StringUtil::trim(const std::string &src, const std::string &c)
{
    return rtrim(ltrim(src, c), c);
}

std::string StringUtil::ltrim(const std::string &src, const std::string &c)
{
    std::string tmp;
    if (ltrim(src, tmp, c) < 0) {
        return src;
    }
    return tmp;
}

std::string StringUtil::rtrim(const std::string &src, const std::string &c)
{
    std::string tmp;
    if (rtrim(src, tmp, c) < 0) {
        return src;
    }
    return tmp;
}

std::string StringUtil::num_to_string(int num)
{
    std::ostringstream oss;
    oss.clear();
    oss << num;
    return oss.str();
}

uint64_t StringUtil::string_to_num(std::string &str, uint8_t base)
{
    uint64_t result = 0;
    std::string nstr = trim(str);
    std::istringstream ss(nstr);
    if (base == 10) {
        ss >> std::dec >> result;
    } else if (base == 16) {
        ss >> std::hex >> result;
    } else if (base == 8) {
        ss >> std::oct >> result;
    } else if (base == 2) {
        size_t n = nstr.size();
        const char * p = nstr.c_str();
        for (size_t i = 0; i < n; ++i) {
            uint64_t tmp = (p[i] - '0');
            result = 2 * result + tmp;
        }
    }
    return result;
}

int StringUtil::split(const std::string &src, const std::string &delimiter,
        int max_items, std::vector<std::string> * result)
{
    size_t cur = 0;
    size_t next = 0;

    while (next != std::string::npos &&
            (max_items < 0 || max_items > static_cast<int>(result->size()) + 1)) {
        next = src.find_first_of(delimiter, cur);
        result->push_back(src.substr(cur, next - cur));
        cur = next + delimiter.size();
    }

    if (next != std::string::npos) { // over max items, and fill the last one
        result->push_back(src.substr(cur));
    }
    return 0;
}

int StringUtil::lower(const std::string &src, std::string &output)
{
    output.resize(src.size());
    std::string::iterator end = std::transform(
            src.begin(), src.end(), output.begin(), tolower);
    if (end == output.end()) {
        return 0;
    }
    return -1;
}

int StringUtil::upper(const std::string &src, std::string &output)
{
    output.resize(src.size());
    std::string::iterator end = std::transform(
            src.begin(), src.end(), output.begin(), toupper);
    if (end == output.end()) {
        return 0;
    }
    return -1;
}

std::string StringUtil::lower(const std::string &src)
{
    std::string res;
    if (lower(src, res) < 0) {
        return src;
    }
    return res;
}

std::string StringUtil::upper(const std::string &src)
{
    std::string res;
    if (upper(src, res) < 0) {
        return src;
    }
    return res;
}

std::string StringUtil::hex(unsigned char c)
{
    std::string result;
    result.reserve(2);
    result.push_back(HEX_CHARS[c >> 4]);
    result.push_back(HEX_CHARS[c & 0xf]);
    return result;
}

std::string StringUtil::hex(const std::string &src)
{
    std::stringstream ss;
    ss.clear();
    size_t length = src.size();
    for (size_t i = 0; i < length; ++i) {
        ss << hex(static_cast<unsigned char>(src[i]));
    }
    return ss.str();
}

std::string StringUtil::base64_encode(const std::string &src)
{
    std::stringstream ss;
    ss.clear();

    size_t length = src.size();
    if (src.empty()) {
        return src;
    }

    unsigned char parts[4];
    for (size_t i = 0; i < length; i += 3) {
        parts[0] = (src[i] & 0xfc) >> 2;
        parts[1] = ((src[i] & 0x03) << 4) | (((length > (i + 1) ? src[i + 1] : 0x00) & 0xf0) >> 4);
        parts[2] = length > (i + 1) ? (((src[i + 1] & 0x0f) << 2) |
                (((length > (i + 2) ? src[i + 2] : 0x00) & 0xc0) >> 6)) : 0x40;
        parts[3] = length > (i + 2) ? (src[i + 2] & 0x3f) : 0x40;

        for (int j = 0; j < 4; ++j) {
            ss << BASE64_CHARS[parts[j]];
        }
    }
    return ss.str();
}

unsigned char StringUtil::base64_decode_char(const unsigned char c)
{
    unsigned char result = 0;
    if (isupper(c)) {
        result = static_cast<unsigned char>(c - 'A');
    } else if (islower(c)) {
        result = static_cast<unsigned char>(c - 'a' + 26);
    } else if (isdigit(c)) {
        result = static_cast<unsigned char>(c - '0' + 52);
    } else if ('+' == c) {
        result = 62;
    } else if ('/' == c) {
        result = 63;
    } else if ('=' == c) {
        result = 64;
    } else {
        result = UCHAR_MAX;
    }
    return result;
}

std::string StringUtil::base64_decode(const std::string &src)
{
    std::stringstream ss;
    ss.clear();
    size_t length = src.size();
    if (src.empty()) {
        return src;
    }

    unsigned char parts[4];
    for (size_t i = 0; i < length; i += 4) {
        parts[0] = base64_decode_char(src[i]);
        parts[1] = length > (i + 1) ? base64_decode_char(src[i + 1]) : 64;
        parts[2] = length > (i + 2) ? base64_decode_char(src[i + 2]) : 64;
        parts[3] = length > (i + 3) ? base64_decode_char(src[i + 3]) : 64;

        ss << static_cast<unsigned char>(((parts[0] << 2) & 0xfc) | ((parts[1] >> 4) & 0x03));
        if (64 == parts[2]) {
            break;
        }
        ss << static_cast<unsigned char>(((parts[1] << 4) & 0xf0) | ((parts[2] >> 2) & 0x0f));
        if (64 == parts[3]) {
            break;
        }
        ss << static_cast<unsigned char>(((parts[2] << 6) & 0xc0) | (parts[3] & 0x3f));
    }
    return ss.str();
}

std::string StringUtil::url_encode(const std::string &src, bool encode_slash)
{
    std::stringstream ss;
    ss.clear();
    size_t length = src.size();
    for (size_t i = 0; i < length; ++i) {
        unsigned char c = src[i];
        if (isalnum(c) || c == '_' || c == '-' || c == '~' || c == '.' || c == '&') {
            ss << c;
        } else if (c == '/' && !encode_slash) {
            ss << c;
        } else {
            ss << '%' << hex(c);
        }
    }
    return ss.str();
}

std::string StringUtil::url_decode(const std::string &src)
{
    std::stringstream ss;
    ss.clear();
    size_t length = src.size();
    for (size_t i = 0; i < length; ++i) {
        if (src[i] != '%') {
            ss << src[i];
        } else {
            unsigned char c = 0;
            if (length <= ++i) {
                break;
            }
            c += isdigit(src[i]) ? (src[i] - '0') :
                    isupper(src[i]) ? (src[i] - 'A' + 0x0a) :
                        islower(src[i]) ? (src[i] - 'a' + 0x0a) : 0x00;
            if (length <= ++i) {
                break;
            }
            c *= 16;
            c += isdigit(src[i]) ? (src[i] - '0') :
                    isupper(src[i]) ? (src[i] - 'A' + 0x0a) :
                        islower(src[i]) ? (src[i] - 'a' + 0x0a) : 0x00;
            ss << c;
        }
    }
    return ss.str();
}

// Definition for return code
const char * stringfy_ret_code(int code)
{
    switch (code) {
        case RET_OK:
            return "OK";
        case RET_SERVICE_ERROR:
            return "BOS service occurs error";
        case RET_CLIENT_ERROR:
            return "BOS client occurs error";
        case RET_INIT_CURL_FAIL:
            return "Init CURL Net handle failed";
        case RET_ILLEGAL_ARGUMENT:
            return "argument is illegal";
        case RET_FILE_INVALID:
            return "file pointer is invalid";
        case RET_ILLEGAL_OPERATION:
            return "operation is illegal";
        default:
            return "OK";
    }
}

END_NAMESPACE
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
