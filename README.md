# http4cpp - A http request/response programming framework implemented by C++

http4cpp is a easily used programming framework under Linux and Mac OS(no support
for Window system). It is based on the libcurl for the only purpose to construct
a easily and friendly used programming model. One can include the header file for
own derivation and extension.

## Requirements

1. Linux 2.6+ and Mac OS 10.10+
2. g++ 4.8+
3. libcurl must be installed:
```shell
    # for debian
    sudo apt-get install libcurl3

    # for redhat
    yum install curl curl-devel
```

## Install

http4cpp can be used by following 3 methods:

### 1. As a third part source code

The `src` directory must be in the compile include path `-I` option.

### 2. As a static library

Use the following command:
```shell
    make static
```
It will generate an directory called `output` in the current directory,
and the static library `libhttp4cpp.a` will be in it.

### 3. As a shared object

Use the following command:
```shell
    make shared
```
It will generate the `libhttp4cpp.so` in the `output` directory.

## Usage

```c++
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
```

The details can be found in the `test/http_test.cpp`. Use
```shell
    make test
```
to generate the test executables. Further extension usage can be
implemented by derivate the HttpRequest and HttpResponse class.
It's waiting for your digging.

------
- Contact: dualyangsong@gmail.com
- Copyrights 2016 (c) Oshyn Song

