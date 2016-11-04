/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_COMMON_STREAM_H
#define HTTP4CPP_COMMON_STREAM_H

#include <stdint.h>

#include <string>

#include "common/common.h"

BEGIN_NAMESPACE

class Stream {
public:
    virtual ~Stream()
    {
        // nothing to do
    }

    virtual std::string get_error_description(int error_code) const
    {
        (void)error_code;
        return "";
    }
};

class InputStream : public Stream {
public:
    virtual ~InputStream()
    {
        // nothing to do
    }

    virtual int64_t read(int64_t size, std::string *data) = 0;
    virtual int64_t read(char *buffer, int64_t size) = 0;
    virtual int64_t get_size() const = 0;
    virtual int64_t seek(int64_t pos) = 0;
    virtual int64_t get_pos() const = 0;
};

class OutputStream : public Stream {
public:
    virtual ~OutputStream()
    {
        // nothing to do
    }

    virtual int64_t write(const std::string &data) = 0;
    virtual int64_t write(const char *buffer, int64_t size) = 0;
    virtual int64_t reserve(int64_t size) = 0;
    virtual int64_t read(uint64_t start, int64_t length, std::string *data) const = 0;
};

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
