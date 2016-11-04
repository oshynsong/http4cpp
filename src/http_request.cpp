/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include "http_request.h"

BEGIN_NAMESPACE

HttpRequest::HttpRequest() :
    _in_stream(NULL),
    _url(""),
    _headers(),
    _method(HTTP_METHOD_INVALID),
    _timeout(-1)
{
    // Nothint to do
}

HttpRequest::~HttpRequest()
{
    _in_stream = NULL;
    _url.clear();
    _headers.clear();
    _method = HTTP_METHOD_INVALID;
    _timeout = -1;
}

int HttpRequest::get_all_headers(std::vector<std::string> *header) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.begin();
    for (; it != _headers.end(); it++) {
        header->push_back(it->first + ":" + it->second);
    }

    return 0;
}

END_NAMESPACE
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
