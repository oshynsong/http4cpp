#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "common/common.h"
#include "common/util.h"
#include "common/memory_stream.h"
#include "common/file_stream.h"

BEGIN_NAMESPACE

log_level_t g_log_level = LOG_LEVEL_DEBUG;
bool g_log_behind       = false;

void test_util()
{
    printf("now : %lld\n", (long long)TimeUtil::now());
    printf("now_ms : %lld\n", (long long)TimeUtil::now_ms());
    printf("now_us : %lld\n", (long long)TimeUtil::now_us());
    printf("now_utctime: %s\n", TimeUtil::now_utctime().c_str());
    printf("now_gmttime: %s\n", TimeUtil::now_gmttime().c_str());

    std::string utc = TimeUtil::timestamp_to_utctime(TimeUtil::now() + 3600);
    printf("%lld\n", (long long)TimeUtil::utctime_to_timestamp(utc));

    std::string gmt = TimeUtil::timestamp_to_gmttime(TimeUtil::now() + 3600);
    printf("%lld\n", (long long)TimeUtil::gmttime_to_timestamp(gmt));

    printf("%lld\n", (long long)TimeUtil::get_utc_offset());

    ERROR("%d", 123456);
    FATAL("%d", 11111);
    std::string a="1234567890";
    FATAL("%s", a.c_str());
}
/*
void test_string_util()
{
    std::string a = "12345abcedf";
    std::string b = " \t abc中文 ";

    std::cout << "Test trim:\n";
    std::cout << StringUtil::ltrim(b) << '\n';
    std::cout << StringUtil::rtrim(b) << '\n';
    std::cout << StringUtil::trim(b) << '\n';

    std::cout << "Test lower upper hex:\n";
    std::cout << StringUtil::lower(a) << '\n';
    std::cout << StringUtil::upper(a) << '\n';
    std::cout << StringUtil::hex(a) << '\n';

    std::cout << "Test base64 encode/decode:\n";
    std::string enc = StringUtil::base64_encode("a");
    std::cout << enc << '\n';
    std::cout << StringUtil::base64_decode(enc) << '\n';
    enc = StringUtil::base64_encode("ab");
    std::cout << enc << '\n';
    std::cout << StringUtil::base64_decode(enc) << '\n';
    enc = StringUtil::base64_encode("abc");
    std::cout << enc << '\n';
    std::cout << StringUtil::base64_decode(enc) << '\n';
    enc = StringUtil::base64_encode(b);
    std::cout << enc << '\n';
    std::cout << StringUtil::base64_decode(enc) << '\n';

    std::cout << "Test url encode/decode:\n";
    std::cout << StringUtil::url_encode(b) << '\n';
    std::cout << StringUtil::url_decode(b) << '\n';
    std::cout << StringUtil::url_encode(a) << '\n';
    std::cout << StringUtil::url_decode(a) << '\n';
    std::cout << StringUtil::url_decode("==") << '\n';

    std::cout << "Test hmac encode:\n";
    std::string hmac;
    StringUtil::hmac(HMAC_TYPE_SHA1, "key", b, &hmac);
    std::cout << hmac.c_str() << '\n';

    std::cout << StringUtil::sha256hex(b, "11111")<< '\n';

    std::vector<std::string> res;
    std::string delimiter = " ";
    StringUtil::split(b, delimiter, 3, &res);
    for (size_t i = 0; i < res.size(); ++i) {
        std::cout << res[i] << '|';
    }
    std::cout << '\n';
}
*/

END_NAMESPACE

int main(int argc, char ** argv)
{
    http4cpp_ns::test_util();
    //cppsdk_ns::test_string_util();
    return 0;
}
