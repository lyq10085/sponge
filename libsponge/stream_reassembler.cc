#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _unassembled_bytes(capacity), _output(capacity), _capacity(capacity) {}



void StreamReassembler::push_substring_unassembled(std::string_view data, const std::uint64_t index) {
    std::string _data(data);
    
    
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // DUMMY_CODE(data, index, eof);


    if(_eof) return;
    if(eof) _eof = eof;
    if(index == _output.bytes_written()) {
        size_t written = _output.write(data);
        if(written < data.size()) {
            // push to unassembled area(todo) 
        }
    } 


}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes.buffer_size(); }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
