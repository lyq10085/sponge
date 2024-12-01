#include "tcp_receiver.hh"
#include "wrapping_integers.hh"
#include <cstdint>

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    DUMMY_CODE(seg);
}

optional<WrappingInt32> TCPReceiver::ackno() const { return _ackno; }

size_t TCPReceiver::window_size() const { 
    // todo 
    uint64_t x = unwrap(_ackno.value(), _isn.value(), 0ull);
    uint64_t y = unwrap(wrap(_reassembler.stream_out().bytes_read(), _isn.value()), _isn.value(), 0ull);
    return y - x;
}
