/**
 * A http programming framework implemented by C++ based on libcurl
 *
 * Copyright 2016 (c), Oshyn Song (dualyangsong@gmail.com)
 *
 * Distributed under the Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef HTTP4CPP_COMMON_MEMORY_STREAM_H
#define HTTP4CPP_COMMON_MEMORY_STREAM_H

#include <string.h>     /* for memcpy */

#include "common/common.h"
#include "common/stream.h"

BEGIN_NAMESPACE

class MemoryInputStream : public InputStream {
public:
    MemoryInputStream(const void *buffer, int64_t size) : _buffer(buffer), _size(size), _pos(0)
    {
        // nothing to do
    }

    virtual ~MemoryInputStream()
    {
        // nothing to do
    }

    virtual int64_t read(int64_t size, std::string *data)
    {
        int64_t real_size = calculate_real_copy_size(size);
        data->assign(reinterpret_cast<const char *>(_buffer) + _pos, real_size);

        _pos += real_size;
        return real_size;
    }

    virtual int64_t read(char *buffer, int64_t size)
    {
        int64_t real_size = calculate_real_copy_size(size);
        memcpy(buffer, reinterpret_cast<const char *>(_buffer) + _pos, real_size);

        _pos += real_size;
        return real_size;
    }

    virtual int64_t get_size() const
    {
        return _size;
    }

    virtual int64_t seek(int64_t pos)
    {
        if (pos < 0 || pos > _size) {
            return -1;
        }
        _pos = pos;
        return pos;
    }

    virtual int64_t get_pos() const
    {
        return _pos;
    }

private:
    const void   *_buffer;
    int64_t       _size;
    int64_t       _pos;

    int64_t calculate_real_copy_size(int64_t req_size)
    {
        int64_t left_data_size = _size - _pos;
        return left_data_size < req_size ? left_data_size : req_size;
    }
};

class MemoryOutputStream : public OutputStream {
public:
    MemoryOutputStream(void *buffer, int64_t size) : _buffer(buffer), _size(size), _pos(0)
    {
        // nothing to do
    }

    virtual ~MemoryOutputStream()
    {
        // nothing to do
    }

    virtual int64_t write(const std::string &data)
    {
        return write(data.data(), data.size());
    }

    virtual int64_t write(const char *buffer, int64_t size)
    {
        if (size + _pos > _size) {
            return -1;
        }

        memcpy(reinterpret_cast<char *>(_buffer) + _pos, buffer, size);
        _pos += size;

        return size;
    }

    std::string get_buffer_string() const
    {
        return std::string(reinterpret_cast<char *>(_buffer), _pos);
    }

    virtual int64_t reserve(int64_t size)
    {
        if (size <= _size) {
            return 0;
        }

        return -1;
    }

    virtual int64_t read(uint64_t start, int64_t length, std::string *data) const
    {
        if (start > static_cast<uint64_t>(_pos)) {
            return 0;
        }

        int64_t left_length = _pos - start;
        if (length > left_length || length < 0) {
            length = left_length;
        }

        data->assign(reinterpret_cast<char *>(_buffer) + start, length);
        return length;
    }

private:
    void   *_buffer;
    int64_t _size;
    int64_t _pos;
};

class StringOutputStream : public OutputStream {
public:
    explicit StringOutputStream(std::string *buffer) : _buffer(buffer)
    {
        // nothing to do
    }

    virtual int64_t write(const std::string &data)
    {
        return write(data.data(), data.size());
    }

    virtual int64_t write(const char *buffer, int64_t size)
    {
        _buffer->append(buffer, size);
        return size;
    }

    virtual int64_t reserve(int64_t size)
    {
        _buffer->reserve(size);
        return 0;
    }

    virtual int64_t read(uint64_t start, int64_t length, std::string *data) const
    {
        if (start > _buffer->size()) {
            return 0;
        }

        int64_t left_length = _buffer->size() - start;
        if (length > left_length || length < 0) {
            length = left_length;
        }

        data->assign(_buffer->c_str() + start, length);
        return length;
    }

private:
    std::string *_buffer;
};

END_NAMESPACE
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
