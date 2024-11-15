#include "stream_reassembler.hh"

#include "buffer.hh"

#include <bits/stdint-uintn.h>
#include <cstddef>
#include <iterator>
#include <optional>
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
    _unassembled_bytes.push_range(data, index);
    while (auto buf = _unassembled_bytes.read(_output.bytes_written(), 0)) {
        if (buf.has_value()) {
            size_t remaining = _output.remaining_capacity();
            (void)remaining;
            // todo(_unassembled_bytes should support read no more than xxx byte method)
            size_t written = _output.write(buf->copy());
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

void UncontinuousByteRanges::push_range(std::string data, uint64_t index) {
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
    // read string index greater or equal than start_index
    // update _size
    (void)n;
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
            Buffer buf = it->second;
            _size -= it->second.size();
            buf.remove_prefix(start_index - l);
            it = _ranges.erase(it);
            return buf;
        }
    }
    return std::nullopt;
}

bool UncontinuousByteRanges::empty() const { return _ranges.empty(); }