#include "stream_reassembler.hh"

#include "buffer.hh"

#include <bits/stdint-uintn.h>
#include <cstddef>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unistd.h>
#include <utility>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _unassembled_bytes(capacity), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (data.empty()) {
        if (_eof) {
            if (empty())
                _output.end_input();
        } else if (eof) {
            _eof = eof;
            if (empty())
                _output.end_input();
        }
        return;
    }

    std::string_view dup(data);
    size_t remaining = _output.remaining_capacity();

    // eof condition, eof at some 'index'
    if (_eof) {
        if (_unassembled_bytes.empty()) {  // eof at _output.bytes_written()
            return;
        } else if (index >= _unassembled_bytes.end()) {  // below all are eof at _unassembled_bytes.end()
            return;
        } else if (index + data.size() <= _unassembled_bytes.end()) {
            // fast path
            if (_output.bytes_written() == index && remaining >= data.size()) {
                _output.write(data);
            }
        } else {
            dup = dup.substr(0, _unassembled_bytes.end() - index);
        }
    } else if (eof) {
        _eof = eof;
        // fast path
        if (_output.bytes_written() == index && remaining >= data.size()) {
            _output.write(data);
        }
    }

    remaining = _output.remaining_capacity();

    // capacity condition
    if (_output.bytes_written() == index) {
        if (remaining >= dup.size()) {
            _output.write(std::string(dup));  // 只有这一种情况 当前函数不需要拷贝原始data
        } else {
            _output.write(std::string(dup.substr(0, remaining)));
            _unassembled_bytes.push_range(std::string(dup.substr(remaining, data.size() - remaining)),
                                          _output.bytes_written());
        }
    } else if (_output.bytes_written() > index && _output.bytes_written() < index + data.size()) {
        if (remaining >= data.size() - _output.bytes_written() + index) {
            _output.write(std::string(
                dup.substr(_output.bytes_written() - index, data.size() - _output.bytes_written() + index)));
        } else {
            _output.write(std::string(dup.substr(_output.bytes_written() - index, remaining)));
            _unassembled_bytes.push_range(
                std::string(dup.substr(_output.bytes_written() - index + remaining,
                                       data.size() - _output.bytes_written() + index - remaining)),
                _output.bytes_written());
        }
    } else if (_output.bytes_written() >= index + data.size()) {
        (void)0;
    } else {
        _unassembled_bytes.push_range(std::string(dup), index);
    }
    remaining = _output.remaining_capacity();
    while (remaining) {
        std::optional<Buffer> buf = _unassembled_bytes.read(_output.bytes_written(), remaining);
        if (buf.has_value()) {
            size_t written = _output.write(buf->copy());
            remaining = _output.remaining_capacity();
            (void)written;
        } else {
            break;
        }
    }

    // end input of underlying bytestream
    if (empty() && _eof) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes.size(); }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }

void UncontinuousByteRanges::push_range(std::string &&data, uint64_t index) {
    Buffer buf(std::move(data));
    size_t remaining = _cap - size();
    if (remaining <= 0)
        return;
    if (remaining < buf.size()) {
        buf.set_prefix(remaining);
    }
    for (auto it = _ranges.begin(); it != _ranges.end(); it++) {
        if (it->first > index) {
            // std::prev(it) --<data>-- it
            // 1. remove prefix of it
            do {
                uint64_t l = it->first + it->second.start_offset();
                uint64_t r = l + it->second.size();
                uint64_t x = index + buf.size();
                if (x >= r) {
                    it = _ranges.erase(it);
                } else if (x <= l) {
                    break;
                } else {
                    it->second.remove_prefix(x - l);
                    break;
                }
            } while (it != _ranges.end());

            // 2. remove prefix of data
            // check if prev node exists
            if (it != _ranges.begin()) {
                auto pre = std::prev(it);
                uint64_t l = index;
                uint64_t r = l + buf.size();
                uint64_t x = pre->first + pre->second.start_offset() + pre->second.size();
                if (x >= r) {
                    return;
                } else if (x <= l) {
                    (void)0;
                } else {
                    buf.remove_prefix(x - l);
                }
            }

            _ranges.insert(it, std::make_pair(index, std::move(buf)));
            return;
        }
    }

    // deal with overlap, then calculate _size
    if (_ranges.size() >= 1) {
        uint64_t l = _ranges.back().first + _ranges.back().second.start_offset();
        uint64_t r = l + _ranges.back().second.size();
        uint64_t x = index + buf.size();
        // must be index >= l, x >= l
        if (x <= r) {
            return;
        } else if (index < r) {
            buf.remove_prefix(r - index);
        } else {
            (void)0;
        }
    }
    _ranges.push_back(std::make_pair(index, std::move(buf)));
}

// todo(size should be the actual size after compact)
size_t UncontinuousByteRanges::size() const {
    size_t sz{};
    for (auto it = _ranges.begin(); it != _ranges.end(); it++) {
        sz += it->second.size();
    }
    return sz;
}

std::optional<Buffer> UncontinuousByteRanges::read(uint64_t start_index, const size_t n) {
    // read no more than `n`(>0) bytes whose index are greater or equal than start_index
    if (_ranges.empty()) {
        return std::nullopt;
    }
    auto it = _ranges.begin();
    while (it != _ranges.end()) {
        if (it->second.size() == 0) {
            it = _ranges.erase(it);
            continue;
        }

        uint64_t l = it->first + it->second.start_offset();  // 最新的offset
        uint64_t r = l + it->second.size();
        if (l > start_index)
            break;
        if (r <= start_index) {
            it = _ranges.erase(it);
        } else {
            // 这里不对啊
            it->second.remove_prefix(start_index - l);
            Buffer buf = it->second;
            if (buf.size() <= n) {
                it = _ranges.erase(it);
            } else {
                buf.set_prefix(n);
                it->second.remove_prefix(n);
            }
            return buf;
        }
    }
    return std::nullopt;
}

bool UncontinuousByteRanges::empty() const { return size() == 0; }

uint64_t UncontinuousByteRanges::end() const {
    if (empty())
        throw std::logic_error("empty ranges");
    return _ranges.back().first + _ranges.back().second.size();
}