/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_COMMON_FILE_STREAM_H
#define HTTP4CPP_COMMON_FILE_STREAM_H

#include <errno.h>     /* for errno */
#include <stdio.h>     /* for standard C file IO */
#include <sys/stat.h>  /* for low level file IO */
#include <unistd.h>    /* for low level file IO */

#include "common/common.h"
#include "common/stream.h"
#include "common/util.h"

BEGIN_NAMESPACE

class FileInputStream : public InputStream {
public:
    explicit FileInputStream(const std::string &file_name)
    {
        _file_handle = fopen(file_name.c_str(), "r");
    }

    explicit FileInputStream(int fd)
    {
        _file_handle = fdopen(fd, "r");
    }

    virtual ~FileInputStream()
    {
        if (_file_handle != NULL) {
            fclose(_file_handle);
            _file_handle = NULL;
        }
    }

    virtual int64_t read(int64_t size, std::string *data)
    {
        char *buffer = new char[size + 5];
        int64_t data_len = read(buffer, size);
        if (data_len > 0) {
            data->assign(buffer, data_len);
        }

        delete []buffer;
        return data_len;
    }

    virtual int64_t read(char *buffer, int64_t size)
    {
        if (_file_handle == NULL) {
            return -RET_FILE_INVALID;
        }

        int64_t ret = fread(buffer, 1, size, _file_handle);
        if (ret < 0) {
            return -errno;
        }

        return ret;
    }

    virtual int64_t get_size() const
    {
        if (_file_handle == NULL) {
            return -RET_FILE_INVALID;
        }

        struct stat stat_buffer;
        int ret = fstat(fileno(_file_handle), &stat_buffer);
        if (ret != 0) {
            return -errno;
        }

        return stat_buffer.st_size;
    }

    virtual int64_t seek(int64_t pos)
    {
        if (_file_handle == NULL) {
            return -RET_FILE_INVALID;
        }

        int ret = fseek(_file_handle, pos, SEEK_SET);
        if (ret != 0) {
            return -errno;
        }

        return ret;
    }

    virtual int64_t get_pos() const
    {
        int64_t pos =  ftell(_file_handle);
        if (pos < 0) {
            return -errno;
        }

        return pos;
    }

    virtual std::string get_error_description(int error_code) const
    {
        if (error_code == -RET_FILE_INVALID) {
            return "Open file failed";
        }

        const int buffer_size = 1024;
        char buffer[buffer_size];

        if (strerror_r(error_code, buffer, buffer_size)) {
            return "Unknown error";
        }
        return std::string(buffer);
    }

private:
    FILE *    _file_handle;
    int64_t   _file_size;
};

class FileOutputStream : public OutputStream {
public:
    explicit FileOutputStream(const std::string &file_name)
    {
        _file_handle = fopen(file_name.c_str(), "w+");
    }

    explicit FileOutputStream(int fd)
    {
        _file_handle = fdopen(fd, "w+");
    }

    virtual ~FileOutputStream()
    {
        if (_file_handle != NULL) {
            fclose(_file_handle);
            _file_handle = NULL;
        }
    }

    virtual int64_t write(const std::string &data)
    {
        return write(data.c_str(), data.size());
    }

    virtual int64_t write(const char *buffer, int64_t size)
    {
        if (_file_handle == NULL) {
            return -RET_FILE_INVALID;
        }

        return fwrite(buffer, 1, size, _file_handle);
    }

    virtual int64_t reserve(int64_t size)
    {
        (void)size;
        return 0;
    }

    virtual int64_t read(uint64_t start, int64_t length, std::string *data) const
    {
        (void)start;
        (void)length;
        (void)data;
        return -RET_ILLEGAL_OPERATION;
    }

    virtual std::string get_error_description(int error_code) const
    {
        if (error_code == -RET_FILE_INVALID) {
            return "Open File fail";
        }

        if (error_code == -RET_ILLEGAL_OPERATION) {
            return "Operation not allow";
        }

        const int buffer_size = 1024;
        char buffer[buffer_size];

        if (strerror_r(error_code, buffer, buffer_size)) {
            return "Unknown error";
        }
        return std::string(buffer);
    }

private:
    FILE *_file_handle;
};

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
