#include "wrapping_integers.hh"
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <cmath>
#include <cstdint>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;


inline uint64_t dis(uint64_t a, uint64_t b)  {
    return (a > b) ? (a - b) : (b - a);
};

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // DUMMY_CODE(n, isn);

    return WrappingInt32{static_cast<uint32_t>(((n+static_cast<uint64_t>(isn.raw_value())) % (1ul<<32)))};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // DUMMY_CODE(n, isn, checkpoint);
    uint64_t factor = checkpoint / (1ul << 32);
    int32_t offset = n - isn;
    uint64_t cand1 = factor * (1ul<<32) + offset;
    uint64_t cand2 = (factor - 1)*(1ul<<32) + offset;
    uint64_t cand3 = (factor + 1)*(1ul<<32) + offset;
    uint64_t min_gap = std::min(std::min(dis(cand1, checkpoint), dis(cand1, checkpoint)), dis(cand3, checkpoint));
    if(min_gap == dis(cand1, checkpoint)) {
        return cand1;
    }else if(min_gap == dis(cand2, checkpoint)) {
        return cand2;
    } else {
        return cand3;
    }
}
