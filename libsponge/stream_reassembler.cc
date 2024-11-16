#include "stream_reassembler.hh"

#include "buffer.hh"

#include <bits/stdint-uintn.h>
#include <cstddef>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <unistd.h>
#include <utility>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _unassembled_bytes(capacity), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // DUMMY_CODE(data, index, eof);
    // todo(should try to write to _output first if possible)
    if (_eof)
        return;
    if (eof)
        _eof = eof;
    
    size_t remaining = _output.remaining_capacity();
    // fast path
    if(_output.bytes_written() == index) {
        if(remaining >= data.size()) {
            _output.write(data);
        } else {
            _output.write(data.substr(0, remaining));
            _unassembled_bytes.push_range(data.substr(remaining, data.size() - remaining), _output.bytes_written());
        }
    } else if(_output.bytes_written() > index && _output.bytes_written() < index + data.size()) {
        if(remaining >= data.size() - _output.bytes_written() + index) {
            _output.write(data.substr(_output.bytes_written() - index, data.size() - _output.bytes_written() + index));
        } else {
            _output.write(data.substr(_output.bytes_written() - index, remaining));
            _unassembled_bytes.push_range(data.substr(_output.bytes_written() - index + remaining, data.size() - _output.bytes_written() + index - remaining), _output.bytes_written());
        }
    } else if(_output.bytes_written() >= index + data.size()) {
        (void)0;
    } else {
        std::string dup = data;
        _unassembled_bytes.push_range(std::move(dup), index);
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

    if (empty() && _eof) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes.size(); }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }

void UncontinuousByteRanges::push_range(std::string&& data, uint64_t index) {
    size_t remaining = _cap - _size;
    if (remaining <= 0)
        return;
    if (remaining < data.size()) {
        data = data.substr(0, remaining);
    }
    _size += data.size();
    for (auto it = _ranges.rbegin(); it != _ranges.rend(); it++) {
        if (it->first <= index) {
            _ranges.insert(std::prev(it).base(), std::make_pair(index, Buffer(std::move(data))));
            return;
        }
    }
    _ranges.push_front(std::make_pair(index, Buffer(std::move(data))));
}

size_t UncontinuousByteRanges::size() const { return _size; }

std::optional<Buffer> UncontinuousByteRanges::read(uint64_t start_index, const size_t n) {
    // read no more than `n`(>0) bytes whose index are greater or equal than start_index
    if (_ranges.empty()) {
        return std::nullopt;
    }
    auto it = _ranges.begin();
    while (it != _ranges.end()) {
        uint64_t l = it->first;
        uint64_t r = l + it->second.size();
        if (l > start_index)
            break;
        if (r < start_index) {
            _size -= it->second.size();
            it = _ranges.erase(it);
        } else {
            it->second.remove_prefix(start_index - l);
            _size -= start_index - l;
            Buffer buf = it->second;
            if(buf.size() <= n) {
                _size -= buf.size();
                it = _ranges.erase(it);
            } else {
                _size -= n;
                buf.set_prefix(n);
                it->second.remove_prefix(n);
            }
            return buf;
        }
    }
    return std::nullopt;
}

bool UncontinuousByteRanges::empty() const { return _ranges.empty(); }