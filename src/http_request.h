/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_REQUEST_HTTP_REQUEST_H
#define HTTP4CPP_REQUEST_HTTP_REQUEST_H

#include <string>
#include <vector>
#include <map>

#include "common/common.h"

BEGIN_NAMESPACE

enum http_method_t {
    HTTP_METHOD_INVALID,
    HTTP_METHOD_PUT,
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_DELETE
};

class InputStream;

class HttpRequest {
public:
    HttpRequest();

    ~HttpRequest();

    void set_input_stream(InputStream *in_stream)
    {
        _in_stream = in_stream;
    }

    InputStream * get_input_stream() const
    {
        return _in_stream;
    }

    void set_http_method(http_method_t method)
    {
        _method = method;
    }

    http_method_t get_http_method() const
    {
        return _method;
    }

    void add_http_header(const std::string &key, const std::string &val)
    {
        _headers[key] = val;
    }

    const std::map<std::string, std::string> & get_http_header() const
    {
        return _headers;
    }

    void set_url(const std::string &url)
    {
        _url = url;
    }

    std::string get_url() const
    {
        return _url;
    }

    int get_timeout() const
    {
        return _timeout;
    }

    void set_timeout(int timeout)
    {
        _timeout = timeout;
    }

    int get_all_headers(std::vector<std::string> *header) const;

private:
    InputStream *                      _in_stream;
    std::string                        _url;
    std::map<std::string, std::string> _headers;
    http_method_t                      _method;
    int                                _timeout;
};

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
