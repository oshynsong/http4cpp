/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include <curl/curl.h>

#include "http/http_client.h"
#include "common/util.h"
#include "common/memory_stream.h"

BEGIN_NAMESPACE

int HttpClient::init()
{
    DEBUG("%s", "call curl_global_init");
    return curl_global_init(CURL_GLOBAL_ALL);
}

int HttpClient::cleanup()
{
    DEBUG("%s", "call curl_global_cleanup");
    curl_global_cleanup();
    return 0;
}

int HttpClient::request(const HttpRequest &request, HttpResponse *response)
{
    CURL *curl_handle = NULL;

    struct curl_slist *http_header_slist = NULL;
    int ret = 0;
    do {

        curl_handle = curl_easy_init();
        if (curl_handle == NULL) {
            curl_global_cleanup();
            return RET_INIT_CURL_FAIL;
        }

        std::string url = request.get_url();
        DEBUG("http_request: url:%s", url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

        // prevent core dump when used in multi-thread application 
        // for the case the libcurl is not built with c-ares
        curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_stream);

        InputStream *req_stream = request.get_input_stream();
        http_method_t http_method = request.get_http_method();
        std::string post_data;

        if (http_method == HTTP_METHOD_PUT) {
            curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_stream);
            curl_easy_setopt(curl_handle, CURLOPT_READDATA, req_stream);
            if (req_stream == NULL) {
                curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE, 0);
            } else {
                curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE,
                        req_stream->get_size() - req_stream->get_pos());
            }
        } else if (http_method == HTTP_METHOD_DELETE) {
            curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
        } else if (http_method == HTTP_METHOD_HEAD) {
            curl_easy_setopt(curl_handle, CURLOPT_NOBODY, true);
        } else if (http_method == HTTP_METHOD_POST) {
            if (req_stream == NULL) {
                curl_easy_setopt(curl_handle, CURLOPT_POST, true);
                curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, 0);
            } else {
                int64_t size = req_stream->get_size() - req_stream->get_pos();
                // !!! CURL_POSTFIELDS must be char *, not string.c_str()(const char *)
                // string converted to char * must be carefully used!
                post_data.reserve(size);
                req_stream->read(size, &post_data);
                curl_easy_setopt(curl_handle, CURLOPT_POST, true);
                curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS,
                        const_cast<char *>(post_data.c_str()));
                curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, size);
            }
        }

        // set http header
        std::vector<std::string > headers;
        request.get_all_headers(&headers);
        for (uint32_t i = 0; i < headers.size(); i++) {
            http_header_slist = curl_slist_append(http_header_slist, headers[i].c_str());
            DEBUG("http_request: header:%s", headers[i].c_str());
        }
        if (http_header_slist != NULL) {
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, http_header_slist);
        }

        int timeout = request.get_timeout();
        if (timeout > 0) {
            curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, timeout);
        }

        int ret = curl_easy_perform(curl_handle);
        if (ret != 0) {
            std::ostringstream oss;
            oss << "curl_easy_perform :ret " << ret;
            WARN("%s", oss.str().c_str());
            std::stringstream ss;
            ss << "Request server fail, ret:" << ret;

            if (curl_handle != NULL) {
                curl_easy_cleanup(curl_handle);
            }

            if (http_header_slist != NULL) {
                curl_slist_free_all(http_header_slist);
            }
            curl_global_cleanup();
            ERROR("%S", ss.str().c_str());
            return RET_CLIENT_ERROR;
        }

        ret = RET_OK;
    } while (false);

    if (curl_handle != NULL) {
        curl_easy_cleanup(curl_handle);
    }

    if (http_header_slist != NULL) {
        curl_slist_free_all(http_header_slist);
    }

    return ret;
}

size_t HttpClient::write_stream(void *ptr, size_t size, size_t nmemb, void *stream_handler)
{
    if (stream_handler == NULL) {
        return size * nmemb;
    }

    HttpResponse *response = reinterpret_cast<HttpResponse *>(stream_handler);
    size_t len = size * nmemb;

    return response->write_body(reinterpret_cast<char *>(ptr), len);
}

size_t HttpClient::read_stream(void *ptr, size_t size, size_t nmemb, void *stream)
{
    if (stream == NULL) {
        return 0;
    }

    InputStream *reader = reinterpret_cast<InputStream *>(stream);
    return reader->read(reinterpret_cast<char *>(ptr), size * nmemb);
}

END_NAMESPACE
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
