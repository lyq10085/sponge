#include "tcp_sender.hh"

#include "buffer.hh"
#include "tcp_config.hh"
#include "tcp_segment.hh"
#include "tcp_state.hh"
#include "util.hh"
#include "wrapping_integers.hh"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <random>
#include <type_traits>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {
    _retransmission_timeout = _initial_retransmission_timeout;
    _remain = _retransmission_timeout;
}

uint64_t TCPSender::bytes_in_flight() const {
    // use last ack to calculate bytes in flight
    return _next_seqno - _remote_last_ack;
}

void TCPSender::fill_window() {
    const std::string &status = TCPState::state_summary(*this);
    if (status == TCPSenderStateSummary::ERROR) {
        return;
    } else if (status == TCPSenderStateSummary::CLOSED) {
        TCPSegment syn_seg;
        syn_seg.header().syn = true;
        syn_seg.header().seqno = _isn;
        _segments_out.push(syn_seg);
        _retransmission_queue.push_back(syn_seg);
        _next_seqno = 1;
    } else if (status == TCPSenderStateSummary::SYN_SENT) {
        (void)0;
    } else if (status == TCPSenderStateSummary::SYN_ACKED) {
        // todo(0 size window)
        if (_remote_last_window == 0 && _retransmission_queue.empty()) {
            if (_stream.eof()) {
                TCPSegment seg;
                seg.header().fin = true;
                seg.header().seqno = next_seqno();
                _segments_out.push(seg);
                _retransmission_queue.push_back(seg);
                _next_seqno += seg.length_in_sequence_space();
            } else if (_stream.buffer_empty()) {
                (void)0;
            } else {
                TCPSegment seg;
                seg.payload() = Buffer(_stream.read(1));
                seg.header().seqno = next_seqno();
                _segments_out.push(seg);
                _retransmission_queue.push_back(seg);
                _next_seqno += seg.length_in_sequence_space();
            }
            return;
        }

        uint32_t bytes_in_sequence_space = 0;
        do {
            if (bytes_in_flight() >= _remote_last_window) {
                break;
            }
            if (_stream.eof()) {
                send_empty_segment();
                bytes_in_sequence_space = 1;
                break;
            }
            if (_stream.buffer_empty())
                break;
            size_t sz = _remote_last_window - bytes_in_flight();
            std::string payload =
                _stream.read(std::min(sz, std::min(_stream.buffer_size(), TCPConfig::MAX_PAYLOAD_SIZE)));
            TCPSegment seg;
            seg.payload() = Buffer(std::move(payload));
            seg.header().seqno = next_seqno();

            // piggyback fin
            bytes_in_sequence_space += seg.payload().size();
            if (_stream.eof() && bytes_in_flight() + bytes_in_sequence_space < _remote_last_window) {
                seg.header().fin = true;
                bytes_in_sequence_space++;
            }

            _next_seqno += seg.length_in_sequence_space();
            _segments_out.push(seg);
            _retransmission_queue.push_back(seg);

            if (seg.header().fin) {
                break;
            }

        } while (bytes_in_flight() < _remote_last_window);
    } else if (status == TCPSenderStateSummary::FIN_SENT) {
        (void)0;
    } else {
        (void)0;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    uint64_t ack = unwrap(ackno, _isn, _next_seqno);
    if (ack > _next_seqno) {
        return;
    }
    if (ack > _remote_last_ack) {
        _retransmission_timeout = _initial_retransmission_timeout;
        _remain = _retransmission_timeout;
        _retrasmit = 0;
    }
    _remote_last_window = window_size;
    const std::string &status = TCPState::state_summary(*this);
    _remote_last_ack = std::max(ack, _remote_last_ack);
    if (status == TCPSenderStateSummary::ERROR) {
        (void)0;
    } else if (status == TCPSenderStateSummary::CLOSED) {
        (void)0;
    } else if (status == TCPSenderStateSummary::SYN_SENT || status == TCPSenderStateSummary::SYN_ACKED ||
               status == TCPSenderStateSummary::FIN_SENT) {
        // while(!_segments_out.empty()) {
        //     const TCPSegment& seg = _segments_out.front();
        //     uint64_t seq = unwrap(seg.header().seqno, _isn, _next_seqno);
        //     if(ack >= (seq + ((seg.header().syn || seg.header().fin) ? 1 : seg.payload().size()))) {
        //         _segments_out.pop();
        //     }else {
        //         break;
        //     }
        // }

        // for(auto it = _retransmission_queue.begin(); it != _retransmission_queue.end();) {
        //     const TCPSegment& seg = *it;
        //     uint64_t seq = unwrap(seg.header().seqno, _isn, _next_seqno);
        //     if(ack >= (seq + ((seg.header().syn || seg.header().fin) ? 1 : seg.payload().size()))) {
        //         it = _retransmission_queue.erase(it);
        //     }else {
        //         break;
        //     }
        // }

        while (!_retransmission_queue.empty()) {
            const TCPSegment &seg = _retransmission_queue.front();
            uint64_t seq = unwrap(seg.header().seqno, _isn, _next_seqno);
            if (ack >= (seq + seg.length_in_sequence_space())) {
                _retransmission_queue.pop_front();
            } else {
                break;
            }
        }

        // push to _segments_out if possible
        // fill_window();

    } else {
        (void)0;
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    if (_remain > ms_since_last_tick) {
        _remain -= ms_since_last_tick;
    } else {
        // do restransmission
        if (!_retransmission_queue.empty()) {
            for (auto it = _retransmission_queue.begin(); it != _retransmission_queue.end(); it++) {
                _segments_out.push(*it);
                break;
            }
            auto status = TCPState::state_summary(*this);
            if ((status == TCPSenderStateSummary::SYN_ACKED || status == TCPSenderStateSummary::FIN_SENT) &&
                _remote_last_window == 0) {
                _retransmission_timeout = _initial_retransmission_timeout;
                _remain = _retransmission_timeout;
                _retrasmit++;
                return;
            }

            _retransmission_timeout *= 2;
            _remain = _retransmission_timeout;
            _retrasmit++;
        } else {
            _retrasmit = 0;
            _retransmission_timeout = _initial_retransmission_timeout;
            _remain = _retransmission_timeout;
        }
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return _retrasmit; }

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().fin = true;
    seg.header().seqno = next_seqno();
    _segments_out.push(seg);
    _retransmission_queue.push_back(seg);
    _next_seqno += seg.length_in_sequence_space();
}
