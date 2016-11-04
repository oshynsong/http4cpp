/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_HTTP_HTTP_CLIENT_H
#define HTTP4CPP_HTTP_HTTP_CLIENT_H

#include <map>
#include <string>
#include <sstream>

#include "common/common.h"
#include "common/stream.h"
#include "http_request.h"
#include "http_response.h"

BEGIN_NAMESPACE

class InputStream;
class OutputStream;

class HttpClient {
public:
    static int init();
    static int cleanup();
    static int request(const HttpRequest &request, HttpResponse *response);

private:
    static size_t write_stream(void *ptr, size_t size, size_t nmemb, void *stream);
    static size_t read_stream(void *ptr, size_t size, size_t nmemb, void *stream);
};

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
