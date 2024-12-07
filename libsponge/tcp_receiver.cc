#include "tcp_receiver.hh"

#include "tcp_state.hh"
#include "wrapping_integers.hh"

#include <cstdint>
#include <optional>

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    // DUMMY_CODE(seg);
    std::string status = TCPState::state_summary(*this);
    if (status == TCPReceiverStateSummary::ERROR) {
        return;
    } else if (status == TCPReceiverStateSummary::LISTEN) {
        if (seg.header().syn || seg.header().rst) {
            _ackno = 1;
            _isn_recv = seg.header().seqno;
            if (seg.header().fin) {
                _reassembler.push_substring(seg.payload().copy(), 0, true);
                _ackno = 2;
            } else {
                _reassembler.push_substring(seg.payload().copy(), 0, false);
            }
            _checkpoint = _reassembler.stream_out().bytes_written();
        }
    } else if (status == TCPReceiverStateSummary::FIN_RECV) {
        return;
    } else {
        // SYN_RECV
        uint64_t index = unwrap(seg.header().seqno, _isn_recv, _checkpoint);
        if (seg.header().fin) {
            _reassembler.push_substring(seg.payload().copy(), index - shift_size(), true);
            _ackno = 2;
        } else {
            _reassembler.push_substring(seg.payload().copy(), index - shift_size(), false);
        }
        _checkpoint = _reassembler.stream_out().bytes_written();
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (_ackno.has_value()) {
        return wrap(_reassembler.stream_out().bytes_written() + shift_size(), _isn_recv);
    }
    return std::nullopt;
}

size_t TCPReceiver::window_size() const {
    if (_ackno.has_value()) {
        return _capacity - _reassembler.stream_out().buffer_size();
    }
    return _capacity;
}

size_t TCPReceiver::shift_size() const {
    if (_ackno.value() == 1 || (_ackno.value() == 2 && _reassembler.unassembled_bytes() == 0))
        return _ackno.value();
    else
        return _ackno.value() - 1;
}
