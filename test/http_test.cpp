#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "common/common.h"
#include "common/memory_stream.h"
#include "common/util.h"
#include "http/http_client.h"
#include "http_request.h"
#include "http_response.h"

BEGIN_NAMESPACE

log_level_t g_log_level = LOG_LEVEL_DEBUG;
bool g_log_behind       = false;

static char buf[4096];

void test_http()
{
    HttpRequest req;
    req.set_timeout(30*1000);
    req.add_http_header("content-type", "text/plain");
    req.set_http_method(HTTP_METHOD_HEAD);
    req.set_url("www.baidu.com");

    std::string sbuf;
    MemoryOutputStream os(buf, 4096);
    HttpResponse res;
    res.set_output_stream(&os);

    HttpClient::init();
    HttpClient::request(req, &res);
    HttpClient::cleanup();

    std::cout << "Status code:" << res.get_http_code() << std::endl;
    std::map<std::string, std::string> headers = res.get_response_header();
    std::cout << res.get_http_version() << std::endl;
    std::cout << res.get_http_code() << std::endl;
    std::cout << "Body:\n" << os.get_buffer_string() << std::endl;
    std::cout << "ErrorMsg:\n" << res.get_error_message() << std::endl;
    std::cout << "Header Size: " << headers.size() << std::endl;
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); ++it) {
        std::cout << "======" << it->first << ":" << it->second << std::endl;
    }
}

END_NAMESPACE

int main(int argc, char ** argv)
{
    http4cpp_ns::test_http();
    return 0;
}
