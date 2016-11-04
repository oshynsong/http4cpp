/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include <string.h>

#include "http_response.h"
#include "common/util.h"
#include "common/memory_stream.h"

BEGIN_NAMESPACE

int HttpResponse::parse_status_line(const std::string &status_line)
{
    std::vector<std::string> items;
    StringUtil::split(status_line, std::string(" "), 3, &items);
    if (items.size() != 3U) {
        std::stringstream ss;
        ss << "status_line format error, status_line:" << status_line;
        ERROR("%s", ss.str().c_str());
        return RET_SERVICE_ERROR;
    }

    _http_version = StringUtil::trim(items[0]);
    std::stringstream ss(StringUtil::trim(items[1]));
    ss >> _http_code;
    _reason_phrase = StringUtil::trim(items[2]);

    DEBUG("Http version : %s", _http_version.c_str());
    DEBUG("Http code : %d", _http_code);
    DEBUG("Reason phrase : %s", _reason_phrase.c_str());
    return 0;
}

int HttpResponse::parse_header_line(const std::string &header_line,
        std::string *key, std::string *value)
{
    std::vector<std::string> items;
    StringUtil::split(header_line, std::string(":"), 2, &items);
    if (items.size() != 2U) {
        std::stringstream ss;
        ss << "parse_header_line error, header_line:" << header_line;
        ERROR("%s", ss.str().c_str());
        return 0;
    }

    key->assign(StringUtil::trim(items[0]));
    value->assign(StringUtil::trim(items[1]));
    return 0;
}

int HttpResponse::write_header(const std::string &line)
{
    DEBUG("receive http header line: %s", StringUtil::trim(line).c_str());
    if (_has_recv_status_line) {
        std::string key;
        std::string value;

        int ret = parse_header_line(line, &key, &value);
        if (ret != 0) {
            return ret;
        }

        if (strncmp("Content-Length", key.c_str(), key.size()) == 0 && _body_stream != NULL) {
            std::stringstream ss(value);
            long long content_length = 0;
            ss >> content_length;
            ret = _body_stream->reserve(content_length);
            if (ret != 0) {
                ERROR("%s", stringfy_ret_code(RET_CLIENT_ERROR));
                return 0;
            }
        }

        _response_headers[key] = value;

        DEBUG("add response header, %s : %s", key.c_str(), value.c_str());
        return 0;
    } else {
        _has_recv_status_line = true;
        return parse_status_line(line);
    }
}

int HttpResponse::write_body(void *ptr, size_t size)
{
    std::string line(reinterpret_cast<char *>(ptr), size);
    if (!_has_recv_header_line) {
        if (line == std::string("\r\n")) {
            _has_recv_header_line = true;
            return 2;
        }
        int ret = write_header(line);
        if (ret) {
            ERROR("parse http header error %d", ret);
        }
        return size;
    } else {
        DEBUG("receive http body, length: %d", line.size());
        if (_http_code < 200 || _http_code >= 300) {
            _error_stream << line;
            return size;
        }

        return _body_stream->write(reinterpret_cast<char *>(ptr), size);
    }
    return size;
}

int HttpResponse::get_response_header(const std::string &key, std::string *data) const
{
    std::map<std::string, std::string>::const_iterator it = _response_headers.find(key);
    if (it == _response_headers.end()) {
        ERROR("get response header failed : %s", key.c_str());
        return -1;
    }
    data->assign(it->second);

    return 0;
}

END_NAMESPACE
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
