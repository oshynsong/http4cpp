/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_HTTP_RESPONSE_H
#define HTTP4CPP_HTTP_RESPONSE_H

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "common/common.h"
#include "common/stream.h"

BEGIN_NAMESPACE

class OutputStream;

class HttpResponse {
public:
    HttpResponse()
    {
        _body_stream = NULL;
        _has_recv_status_line = false;
        _has_recv_header_line = false;
    }

    void set_output_stream(OutputStream * data)
    {
        _body_stream = data;
    }

    OutputStream * get_output_stream() const
    {
        return _body_stream;
    }

    void set_http_version(const std::string & data)
    {
        _http_version = data;
    }

    std::string get_http_version() const
    {
        return _http_version;
    }

    void set_http_code(const int data)
    {
        _http_code = data;
    }

    int get_http_code() const
    {
        return _http_code;
    }

    void set_reason_phrase(const std::string & data)
    {
        _reason_phrase = data;
    }

    std::string get_reason_phrase() const
    {
        return _reason_phrase;
    }

    std::string get_error_message() const
    {
        return _error_stream.str();
    }

    const std::map<std::string, std::string> & get_response_header() const
    {
        return _response_headers;
    }

    int write_header(const std::string &line);
    int write_body(void *ptr, size_t size);
    int get_response_header(const std::string &key, std::string *data) const;

private:
    int parse_status_line(const std::string &status_line);
    int parse_header_line(const std::string &header_line, std::string *key, std::string *value);

    OutputStream *                     _body_stream;
    std::stringstream                  _error_stream;
    std::string                        _http_version;
    int                                _http_code;
    std::string                        _reason_phrase;
    std::map<std::string, std::string> _response_headers;
    bool                               _has_recv_status_line;
    bool                               _has_recv_header_line;
};

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
