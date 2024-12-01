#include "byte_stream.hh"

#include <stdexcept>
#include <string_view>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    // DUMMY_CODE(capacity);
    cap_ = capacity;
}

size_t ByteStream::write(const string &data) {
    // DUMMY_CODE(data);
    size_t written{0};
    size_t remain{remaining_capacity()};
    if (remain >= data.size()) {
        written = data.size();
        std::string bytes(data);
        storage_.append(Buffer(std::move(bytes)));
    } else if (remain > 0) {
        written = remain;
        std::string bytes(data.data(), remain);
        storage_.append(Buffer(std::move(bytes)));
    }
    bytes_written_cnt_ += written;
    return written;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // DUMMY_CODE(len);
    if (buffer_size() < len) {
        throw std::runtime_error("[lyq] buffer size less than what");
        return {};
    }

    std::string bytes = storage_.concatenate(len);
    return {bytes.data(), len};
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    // DUMMY_CODE(len);
    if (buffer_size() < len) {
        throw std::runtime_error("[lyq] buffer size less than what");
    }
    storage_.remove_prefix(len);
    bytes_read_cnt_ += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    // DUMMY_CODE(len);
    std::string bytes = peek_output(len);
    pop_output(len);
    return bytes;
}

void ByteStream::end_input() { eof_ = true; }

bool ByteStream::input_ended() const { return eof_; }

size_t ByteStream::buffer_size() const { return storage_.size(); }

bool ByteStream::buffer_empty() const { return buffer_size() == 0; }

// eof and no bytes in buffer
bool ByteStream::eof() const { return buffer_empty() && eof_; }

size_t ByteStream::bytes_written() const { return bytes_written_cnt_; }

size_t ByteStream::bytes_read() const { return bytes_read_cnt_; }

size_t ByteStream::remaining_capacity() const { return cap_ - buffer_size(); }
