#include "byte_stream.hh"
#include "byte_stream_test_harness.hh"
#include "fsm_stream_reassembler_harness.hh"
#include "stream_reassembler.hh"
#include "util.hh"

#include <exception>
#include <iostream>

using namespace std;

// 长度2629 5353
// aabbbbabbbbabbbaaaaababaabbbbaabbabaaabaabbabbbbabaaabaaabbaabbaaaaaabbbbaabbabbabbaaabbabbabbaababbaaabbbabaaaabbaabaabbbabbaabaaabababbababbaabbbbbbaabbaaaabbbbabaabbbbabaaaaaaabbbbbbabbbbbbbaaaabaaaaaaaabbbbaaababbabaaabbabbaaaabaaabaaabaabaabaabbabbaababbbabbababaabaababbaaabaaaaababaababbabbaaababbbabaaaaaaabaabbbbbaaaaabaabaabaabbbababbaabbabaaaabbbaabbbbaababbabbabbbbbabbbbaabbbaabaaabbaabbbbbbbbbaaabbaaaabababbbaabaabaabbbbbabbaaabbbbababbabababababbabaaabaababbabbaaabaaaababaabbabbbaabbbababaaaaababababbbbbbaaabbabbbaaaabbbabbbbabaabaabbbababbabbbabbabbaaaababbbbaaabaabaabaaaaaababaaaabaababaaabaababababbaaababbbabaababbbbaabbbaababbbaaabbabbbabbabbbbbbbbbbbaabbbabababaaaaabbabaaabbaaabababbbbbbaabaabbaabaabbaaaaaababaabbababababbbbbbabbabbaababbabbbaaaabbbbaaabaaaabbabbbaabbbbaaaabbbbbbaaabababbaaaaabababaabbbaabbaabbababbabaaaabbbaabaababaabaababbaabbbababaaabaaaabaaabbaaaabbbbbaabbbbabbbbaaabbaabbaaababaabbbaaabbabbbabbbaabbabaaaabababaaababbbbaababbbbaaaabbbaaaaabaababbbaaaababaaaaabbbabbbabbbbbbaaaabbbbbbbaabababababbabbaaaabaababaabaaaabaababbabaabbbbbaabaaaabbaabbbabbbababbaaaaabbabbbabababbbabbababbaaabaaaaabaabaababbaababbaaabbaababaaababbbabaabbaababbabaabbabaaaaabaabbbbbaaabbbbabaabbbabbaaababaabaaabbaabbbbabbabaaaaabaaabbaaaaabbbbbabaaaabaaaaabbabbababbbababbabbaabbabbbbabbbabaaababbabbaaabbaaabbaabbabbabbbbaaaabbbbbbabaababbabbaababbaabaaaabaaaabbabbbaaabababaababbbaaabbbbbbababaaabbbbabaaaabbbabbbabbbabbaaaaabbabababaabababbbaababbaababbbbbbabaabbababbbaababbbabaababbabbaaababbbaabbbbbabaaaababbbaaaabbababbbaabbbaaababaaabbbbbbabbabbbbabbbbaaabbabbbabaabbbbbabbabbbabbaaababaabbbbbabbaaabaaabbbaaaaabbabbbaaaabababababababaaabbabbbbbabbbabaaaabbbabaaabbaabbababaaaababbabbaaabbabbbbbaabbbaabbabaaaaababaaabaabbabaaababbbababbabaabaaabababaabbabbbaaaababaaabaaababababaabbbaaababaaabbbababaaaaaaabbbbbbbaabbbaabbbbbbabbaabbbbbabbabababaaaabbbbbaababaababbabbbbbbaaaabbbaaaabbbbaabbaabaaabbbabbabababbabbaabbbaabaaabbaabbabbbabbabbbbbabaabaaabbabbabababbabbbabaaaaabababababaabbbbaabbaabbbbbaabbbaaaaaaaaabbaabbaabbbabaabbaababbababaabbabbbaaaabbbabbabababbabbaaaabbbaaaaabbbabbabbbaaabbaaabbbaaabbaabaabbaababbabaabbbaabaabbaaaaababbbbaaabaabbaaabaabaaaaaabbabaabababbababaaababbbbbbbbbbbaaaaabaababaaabbaaabbbbaababbabbbbbaabaaaabbaaabababbabbabababaabaaaaaabaaaaabaababbabbbaaabbbbabbabaabbabaaabaababbabaabababbbbabaabbabaababbbbbabbabaaaaabaaabababbbababbaabababbabbabbbbababbbbabaababbabbbbbabaaaababbbbaaaaaaabbbabbbbaaaaabbabaaabbabaaaaaaaabaaaabbbbbbaababaaabbaabababaaabbaaabaaababababbaabababbaaabbbbbaabaaabaabbbaaaabaaabaaabaabbabababbabbbabaababbbabbbbbabbabbaaaaaabab
// 1 长度2617 2449
// bbaabaabbbbaababababaaaaabababababbaabbbaabbaababbbaabbbabbbaaababbaabbaabaaaabaabbaabaabbbaabaabbaabbaaaaabbaaabbaaaaaababaabbaaabbbbabaabaababbbabaaaaaabaababbbabbbbaabbabbbabbbabbababaabbaaaaabaaaabaabbaabaaabaaaababbbabbabbababbaaaabbaababbbbbaabbbbababbabaabbabbbbabaababbabaabaaaaaababaaaaaababaabaaababbabababbbbbbbaaaabbbaaaabbbabbaaaabababbbabaabbbbaabbaabaaabaaabbbbabbbbaaabbbaaabaaabbbbbbbaaaabbbbbaaaabaabaaabbaabbbaabaabbabbbabbbbabbaabbabaabaaabbbbbbaaabbaaababbbbaabaaaaabbbbabbbaaaabbbbababbbabaabbaabbabaabaabaaaabbbbbaaaabbababbbabbbabaaaabbaabbbabaaabbbbbbaaababbaaaabaaabbabaabaabbaaaaabaaababbaababaabbabababbbbaabababaaabbabbbbaabaabaabababaabbbbaabbaabbbabaaabbababaababbaabbaaaababbaabbaaabbbbbbaaababaaabaabababaaabbbbaaabbbbbaabbbbbbbbaaabaaabaaabaabbbbabbbbabbabaaabbaaaaaaaaabaaabaabbababaabbbbabbaabaaaaaabbababbababbbbbbaaaaabaabbbbbaaabbbbbabaaabaabbabaabbbbbbbbbbbbaababbbabaaabaabbabaaaaabaabaaaaabbabaabbbbbbbbabbabababaabaaabbbaabaababbbaabbbbbbbabbabbaaaabaaaabaaaaaabbabbabaaaabaaaaaabbbabaaabbaaabbaaabaabbbbaabaaabbabbbabaabbbbbbabbabbaabbbaaabbbbbaaababbaaaaaaaabaaaabbaabbbaabbbbabaaababbaaaaaabbaaababbbabbabbaaabbaaaababbaabaaabbbbababaabbbabbaaabbbbbbabbbabbbbabbabbaaaabaaaaabbabbbbabbaabbbaaaabbbbbbbabbabbaabaaaaaababababbbabaaaababaaababaabbabaabbbbaabbabbabbbbabbbbabbbbabababaaaaaaaabbbaabaaaababaaaabaaaaababaaabaaaabaabaaababbbbaabababbbaabbabaaabbaabaabbbaaaaababbbbbabbbbababababbababbbababaaabaaaaabaabbabaabbbabaaabbabbbbabaabbbbaaaababbaaabababbabbaaaaabbbbbbbbaaabbbbaabbbaaabbaabbaaabbaaabbbabbaabbaaabbbaabaabaabbbbbabbbbbbabbbbaabaabbaabaabbbaaaaabbbaababaabbabbbbbbabaaaabababaabbaabbbabbabbabbabbaabbaababbbabbabababaabbbbabbbabbbabaaabbabbbbaabaaaabaaaabbbaababababbbbbaaabaababbbbabaababbabbbaabbaaababbababaaabbbabbaabbbaaaaaababbbabababaabaaabaaaabaaaabbaaababbbabaabaabbaababbaaaaabbabbaaabababbbababbaaaababbabbaaababbbbbabbbaaababaaabbabbbbbbaabbbbbbaabababbabbaaaabbbbbbaababbbabbbbbabababaaaaababbbaaabaaabbaaaaaabaaaabbabaaaaaaaaababbbaaabaaaabbabaaababaabbbbabaaaabaaaaabbaaaaaabbaabbbbbbbababbbbbbbaabbaaaabbabaaaaababbabaaabbbabababbbabaabbbabaabaaabbbbababbabbabaababababbabaaaababbabbabbbbaaaabbbabaaaaababaaabbbbbbbbababababbabaabaabbabbaabbaaabbbbbbbbbbbbbbbbababbbaaabbababbbbaabbababaabaababbabbabbbabbaabbaaaaaabbaaababaababaabbaaaabaaababaabbbbababbaaabbaaaabbbbababaaaaabbbabbaabbaabbaabbaaaaaabbaaaabbabbabaabbababbaaaaabababaaabaaabbbbbababaabbbaabaaabaabbbbababbbbaaaababaaaababaabbbaabaabaabaaabbbabaabababbaaaaabbaaaabbbabbbabbbbbbbbaaababbbbabbbbabaabbabbaaaabaaabbbabbbaabbabaaaababbbbbbbabbbbabbbabaaabaaababbabbaaabbbbbaba
// 0 长度1184 4910
// aaababaabbbaabaabaabaaabbbabaabababbaaaaabbaaaabbbabbbabbbbbbbbaaababbbbabbbbabaabbabbaaaabaaabbbabbbaabbabaaaababbbbbbbabbbbabbbabaaabaaababbabbaaabbbbbababbbababababaabbabaababbbaababaababbbabbabbabbabbaaabababaaaaaabaaabbaabbbbabaaabbaabaabbbbaabbaaabbababaababaaabbaabbaabbbbabaaaaababbabaaabbababbbaaababbbabbabaabbbbababaaabababaaaabaaabbaaaaaaabbbaabaaabbbabbaaaabababbabaaabaababaababaabbbaaaaabbbabbbaabababbbabaabbaaaababbaabbbaaaabaaabbbbabbbbabbbaaaaababaabbbbaabbabaaabaabbabbbbabaaabaaabbaabbaaaaaabbbbaabbabbabbaaabbabbabbaababbaaabbbabaaaabbaabaabbbabbaabaaabababbababbaabbbbbbaabbaaaabbbbabaabbbbabaaaaaaabbbbbbabbbbbbbaaaabaaaaaaaabbbbaaababbabaaabbabbaaaabaaabaaabaabaabaabbabbaababbbabbababaabaababbaaabaaaaababaababbabbaaababbbabaaaaaaabaabbbbbaaaaabaabaabaabbbababbaabbabaaaabbbaabbbbaababbabbabbbbbabbbbaabbbaabaaabbaabbbbbbbbbaaabbaaaabababbbaabaabaabbbbbabbaaabbbbababbabababababbabaaabaababbabbaaabaaaababaabbabbbaabbbababaaaaababababbbbbbaaabbabbbaaaabbbabbbbabaabaabbbababbabbbabbabbaaaababbbbaaabaabaabaaaaaababaaaabaababaaabaababababbaaababbbabaababbbbaabbbaababbbaaabbabbbabbabbbbbbbbbbbaabbbabababaaaaabbabaaabbaaabababbbbbbaabaabbaabaabbaaaaaababaabba
// 0 长度1463 0
// aaaaaaababbaaaababbbababbaababaabaabaaababaabbbbabbbaaaabbbabaabbbbbababbbabbababbabababbbbbbbbabaabaabaaabbaaabbbbaaaabaaabbbabaabbbbbbaaabaabaaabababbaaaabbaaabbabbbababbaaaabbbbbbbabbaabbbabbbbbaaaabbbabbbaaabbbaababbbbbbbabaabababbbbabaabaaaabbbababbbbaaabbabaaaaaaaababaabbbaabbaaaabbbabbabbabaababbabbababaabbbbaabababaaaabbababaabbbbaabbbbbabbbaababaaaaaaaaaaaabbabababaaabaaaabbaabbbbababbabaaabababaababbababaabbabaabaabbbabbaaaaabbbbaaabababaaabbbabaaabbbbabbababaabbaaaaaaababaabbabbabaabbabbabbabbbbbbbaabbbbabbababaabaabbbabaaabbababaabbbbaabaabbbbaaaabababbbabbbaaaabbaaaabababbbbabbbbbaabbbaabaabaaabbbbbababaabaaabaabbabaaabbaaabbbaaabbaaaaabbbbbbbaabbbbaabaaaaabaaaaabbaababbbababaaabbbbabbabaabbbbbababaaabaaaaabaaabaaababaababaabbbbaababaabaabaaaaaaabaababbabbabababbabbbaababbbaabaabbbbbabbbaaabaaababaabaabbaaababaabbbabbabaabababbaabbaaaababababbaaabaaababbbababababababbabbbbbbbbabaaabaaabaabbababbbbbabaabbabbaabbaababbabbbaabaaabaaaaaabaabbbbbaabbbaaababbabbaaaababbbaababaabaabbaaabaaabbbbabaabaaabaaaabaabaabbaabbbbbaabbaaabaaaaabaababbaabbbbbaabaabbbaabbabaabbbbabbbbaaaabbbbaaaaabbaaabbbababaaabbbbbbabababaaabbabaaaabababbbbbabbabbbbbabbabbbaabbababbaabbbabbbbaaabaaababaaabbbaaabaaaaabbabbababaabbaaaabbbbbbabababbabbabbbbaabbaaaabaabbabbbbbaaabaabaaaababaabbabbaabbbbabbaabaaabbabbabaaaaabbbabbbbbabbaaabbaaabaababbabaabaabaabbaaabaababbbabbbaaaabbbababbbabbaaabbaaabbabbaababababaabaaabbbabbbabbaaaabbbaaabbbabbaaababaababbaabbaab
// 0 长度2263 983
// aaaababbbaababaabaabbaaabaaabbbbabaabaaabaaaabaabaabbaabbbbbaabbaaabaaaaabaababbaabbbbbaabaabbbaabbabaabbbbabbbbaaaabbbbaaaaabbaaabbbababaaabbbbbbabababaaabbabaaaabababbbbbabbabbbbbabbabbbaabbababbaabbbabbbbaaabaaababaaabbbaaabaaaaabbabbababaabbaaaabbbbbbabababbabbabbbbaabbaaaabaabbabbbbbaaabaabaaaababaabbabbaabbbbabbaabaaabbabbabaaaaabbbabbbbbabbaaabbaaabaababbabaabaabaabbaaabaababbbabbbaaaabbbababbbabbaaabbaaabbabbaababababaabaaabbbabbbabbaaaabbbaaabbbabbaaababaababbaabbaabbaaababaaabaabababbabbbbabbbababbbaaaaaaababbababaaaaaaaaaabbabaaabaaababaaabaaabaabbaaabbaaaaabbbbaabbababaabaababbbaabbabbaaaaaaababbbbbbbaabbbbbaaaabbbbbaaaababbabbbaabbabbbbabaabbaababaaabaabaaaaaaaabbbabbbababbbabbabaabababaaabaaaabbbabbbaaabbaaabbbbbaaabbabbabaabbabbaababbbaaabaabbaabbbbbbbaaababbabbaaaabaabbabbaabaaaabaaaaabbbaabaabbabbbaababaaaabbbaababbbaabaabbbbaabbabbaaabaabbbaabaaabbabbabbbaabaaabbabaabbaaabbbbabaaaabaaaaabaaaaabbabbbaaababbbbbbaabbbbbaabababaababbaabaababbbaabababbbaabbbaaabababaaaabaabaabbaaaaaaaabbabbbaaabbaabaaaabbbabaabbaaabbabaaababbbbabbabbaaaaabaaabababbbbabbaaaabbaabbbbbaabbbabaababbbbbaaabaaababaaaaababbbbbbaabbbbbbabbabaabaabbbabbbabaaaabbbbbababbbabbbababbaababbabbbabbbaababbabaaababbabaaabbbabbaaabbabbbabbbbabbbaababbbabbbbbaabbabaaabbbaabbabbbbbabbaaaabaabbaaaaababbaaaabbbabaaababaaabbabaaabbbbbaaabbbbbbaaabbbabaababaabbabbbaabbbaaaababbaaaaabbbbbbbaaaaabbaabbbbbaaababbbababbbbbbababaaabbbaaaaaaaaabbababaaaaabbabbbbaabaaaaabbbbbbbbaabaabbbbaababababaaaaabababababbaabbbaabbaababbbaabbbabbbaaababbaabbaabaaaabaabbaabaabbbaabaabbaabbaaaaabbaaabbaaaaaababaabbaaabbbbabaabaababbbabaaaaaabaababbbabbbbaabbabbbabbbabbababaabbaaaaabaaaabaabbaabaaabaaaababbbabbabbababbaaaabbaababbbbbaabbbbababbabaabbabbbbabaababbabaabaaaaaababaaaaaababaabaaababbabababbbbbbbaaaabbbaaaabbbabbaaaabababbbabaabbbbaabbaabaaabaaabbbbabbbbaaabbbaaabaaabbbbbbbaaaabbbbbaaaabaabaaabbaabbbaabaabbabbbabbbbabbaabbabaabaaabbbbbbaaabbaaababbbbaabaaaaabbbbabbbaaaabbbbababbbabaabbaabbabaabaabaaaabbbbbaaaabbababbbabbbabaaaabbaabbbabaaabbbbbbaaababbaaaabaaabbabaabaabbaaaaabaaababbaababaabbabababbbbaabababaaabbabbbbaabaabaabababaabbbbaabbaabbbabaaabbababaababbaabbaaaababbaabbaaabbbbbbaaababaaabaabababaaabbbbaaabbbbbaabbbbbbbbaaabaaabaaabaabbbbabbbbabbabaaabbaa
// 0

int main() {
    try {
        {
            ReassemblerTestHarness test{2};
            test.execute(BytesAssembled(0));
            test.execute(BytesAvailable(""));
            test.execute(NotAtEof{});
        }

        {
            ReassemblerTestHarness test{5 * 2048};  // cap 10240
            // [5353, 7982)
            test.execute(SubmitSegment(
                "aabbbbabbbbabbbaaaaababaabbbbaabbabaaabaabbabbbbabaaabaaabbaabbaaaaaabbbbaabbabbabbaaabbabbabbaababbaa"
                "abbbabaaaabbaabaabbbabbaabaaabababbababbaabbbbbbaabbaaaabbbbabaabbbbabaaaaaaabbbbbbabbbbbbbaaaabaaaaaa"
                "aabbbbaaababbabaaabbabbaaaabaaabaaabaabaabaabbabbaababbbabbababaabaababbaaabaaaaababaababbabbaaababbba"
                "baaaaaaabaabbbbbaaaaabaabaabaabbbababbaabbabaaaabbbaabbbbaababbabbabbbbbabbbbaabbbaabaaabbaabbbbbbbbba"
                "aabbaaaabababbbaabaabaabbbbbabbaaabbbbababbabababababbabaaabaababbabbaaabaaaababaabbabbbaabbbababaaaaa"
                "babababbbbbbaaabbabbbaaaabbbabbbbabaabaabbbababbabbbabbabbaaaababbbbaaabaabaabaaaaaababaaaabaababaaaba"
                "ababababbaaababbbabaababbbbaabbbaababbbaaabbabbbabbabbbbbbbbbbbaabbbabababaaaaabbabaaabbaaabababbbbbba"
                "abaabbaabaabbaaaaaababaabbababababbbbbbabbabbaababbabbbaaaabbbbaaabaaaabbabbbaabbbbaaaabbbbbbaaabababb"
                "aaaaabababaabbbaabbaabbababbabaaaabbbaabaababaabaababbaabbbababaaabaaaabaaabbaaaabbbbbaabbbbabbbbaaabb"
                "aabbaaababaabbbaaabbabbbabbbaabbabaaaabababaaababbbbaababbbbaaaabbbaaaaabaababbbaaaababaaaaabbbabbbabb"
                "bbbbaaaabbbbbbbaabababababbabbaaaabaababaabaaaabaababbabaabbbbbaabaaaabbaabbbabbbababbaaaaabbabbbababa"
                "bbbabbababbaaabaaaaabaabaababbaababbaaabbaababaaababbbabaabbaababbabaabbabaaaaabaabbbbbaaabbbbabaabbba"
                "bbaaababaabaaabbaabbbbabbabaaaaabaaabbaaaaabbbbbabaaaabaaaaabbabbababbbababbabbaabbabbbbabbbabaaababba"
                "bbaaabbaaabbaabbabbabbbbaaaabbbbbbabaababbabbaababbaabaaaabaaaabbabbbaaabababaababbbaaabbbbbbababaaabb"
                "bbabaaaabbbabbbabbbabbaaaaabbabababaabababbbaababbaababbbbbbabaabbababbbaababbbabaababbabbaaababbbaabb"
                "bbbabaaaababbbaaaabbababbbaabbbaaababaaabbbbbbabbabbbbabbbbaaabbabbbabaabbbbbabbabbbabbaaababaabbbbbab"
                "baaabaaabbbaaaaabbabbbaaaabababababababaaabbabbbbbabbbabaaaabbbabaaabbaabbababaaaababbabbaaabbabbbbbaa"
                "bbbaabbabaaaaababaaabaabbabaaababbbababbabaabaaabababaabbabbbaaaababaaabaaababababaabbbaaababaaabbbaba"
                "baaaaaaabbbbbbbaabbbaabbbbbbabbaabbbbbabbabababaaaabbbbbaababaababbabbbbbbaaaabbbaaaabbbbaabbaabaaabbb"
                "abbabababbabbaabbbaabaaabbaabbabbbabbabbbbbabaabaaabbabbabababbabbbabaaaaabababababaabbbbaabbaabbbbbaa"
                "bbbaaaaaaaaabbaabbaabbbabaabbaababbababaabbabbbaaaabbbabbabababbabbaaaabbbaaaaabbbabbabbbaaabbaaabbbaa"
                "abbaabaabbaababbabaabbbaabaabbaaaaababbbbaaabaabbaaabaabaaaaaabbabaabababbababaaababbbbbbbbbbbaaaaabaa"
                "babaaabbaaabbbbaababbabbbbbaabaaaabbaaabababbabbabababaabaaaaaabaaaaabaababbabbbaaabbbbabbabaabbabaaab"
                "aababbabaabababbbbabaabbabaababbbbbabbabaaaaabaaabababbbababbaabababbabbabbbbababbbbabaababbabbbbbabaa"
                "aababbbbaaaaaaabbbabbbbaaaaabbabaaabbabaaaaaaaabaaaabbbbbbaababaaabbaabababaaabbaaabaaababababbaababab"
                "baaabbbbbaabaaabaabbbaaaabaaabaaabaabbabababbabbbabaababbbabbbbbabbabbaaaaaabab",
                5353));
            test.execute(BytesAvailable(""));
            test.execute(BytesAssembled(0));
            test.execute(UnassembledBytes(2629));
            // [2449,  5066)
            test.execute(SubmitSegment(
                "bbaabaabbbbaababababaaaaabababababbaabbbaabbaababbbaabbbabbbaaababbaabbaabaaaabaabbaabaabbbaabaabbaabb"
                "aaaaabbaaabbaaaaaababaabbaaabbbbabaabaababbbabaaaaaabaababbbabbbbaabbabbbabbbabbababaabbaaaaabaaaabaab"
                "baabaaabaaaababbbabbabbababbaaaabbaababbbbbaabbbbababbabaabbabbbbabaababbabaabaaaaaababaaaaaababaabaaa"
                "babbabababbbbbbbaaaabbbaaaabbbabbaaaabababbbabaabbbbaabbaabaaabaaabbbbabbbbaaabbbaaabaaabbbbbbbaaaabbb"
                "bbaaaabaabaaabbaabbbaabaabbabbbabbbbabbaabbabaabaaabbbbbbaaabbaaababbbbaabaaaaabbbbabbbaaaabbbbababbba"
                "baabbaabbabaabaabaaaabbbbbaaaabbababbbabbbabaaaabbaabbbabaaabbbbbbaaababbaaaabaaabbabaabaabbaaaaabaaab"
                "abbaababaabbabababbbbaabababaaabbabbbbaabaabaabababaabbbbaabbaabbbabaaabbababaababbaabbaaaababbaabbaaa"
                "bbbbbbaaababaaabaabababaaabbbbaaabbbbbaabbbbbbbbaaabaaabaaabaabbbbabbbbabbabaaabbaaaaaaaaabaaabaabbaba"
                "baabbbbabbaabaaaaaabbababbababbbbbbaaaaabaabbbbbaaabbbbbabaaabaabbabaabbbbbbbbbbbbaababbbabaaabaabbaba"
                "aaaabaabaaaaabbabaabbbbbbbbabbabababaabaaabbbaabaababbbaabbbbbbbabbabbaaaabaaaabaaaaaabbabbabaaaabaaaa"
                "aabbbabaaabbaaabbaaabaabbbbaabaaabbabbbabaabbbbbbabbabbaabbbaaabbbbbaaababbaaaaaaaabaaaabbaabbbaabbbba"
                "baaababbaaaaaabbaaababbbabbabbaaabbaaaababbaabaaabbbbababaabbbabbaaabbbbbbabbbabbbbabbabbaaaabaaaaabba"
                "bbbbabbaabbbaaaabbbbbbbabbabbaabaaaaaababababbbabaaaababaaababaabbabaabbbbaabbabbabbbbabbbbabbbbababab"
                "aaaaaaaabbbaabaaaababaaaabaaaaababaaabaaaabaabaaababbbbaabababbbaabbabaaabbaabaabbbaaaaababbbbbabbbbab"
                "abababbababbbababaaabaaaaabaabbabaabbbabaaabbabbbbabaabbbbaaaababbaaabababbabbaaaaabbbbbbbbaaabbbbaabb"
                "baaabbaabbaaabbaaabbbabbaabbaaabbbaabaabaabbbbbabbbbbbabbbbaabaabbaabaabbbaaaaabbbaababaabbabbbbbbabaa"
                "aabababaabbaabbbabbabbabbabbaabbaababbbabbabababaabbbbabbbabbbabaaabbabbbbaabaaaabaaaabbbaababababbbbb"
                "aaabaababbbbabaababbabbbaabbaaababbababaaabbbabbaabbbaaaaaababbbabababaabaaabaaaabaaaabbaaababbbabaaba"
                "abbaababbaaaaabbabbaaabababbbababbaaaababbabbaaababbbbbabbbaaababaaabbabbbbbbaabbbbbbaabababbabbaaaabb"
                "bbbbaababbbabbbbbabababaaaaababbbaaabaaabbaaaaaabaaaabbabaaaaaaaaababbbaaabaaaabbabaaababaabbbbabaaaab"
                "aaaaabbaaaaaabbaabbbbbbbababbbbbbbaabbaaaabbabaaaaababbabaaabbbabababbbabaabbbabaabaaabbbbababbabbabaa"
                "babababbabaaaababbabbabbbbaaaabbbabaaaaababaaabbbbbbbbababababbabaabaabbabbaabbaaabbbbbbbbbbbbbbbbabab"
                "bbaaabbababbbbaabbababaabaababbabbabbbabbaabbaaaaaabbaaababaababaabbaaaabaaababaabbbbababbaaabbaaaabbb"
                "bababaaaaabbbabbaabbaabbaabbaaaaaabbaaaabbabbabaabbababbaaaaabababaaabaaabbbbbababaabbbaabaaabaabbbbab"
                "abbbbaaaababaaaababaabbbaabaabaabaaabbbabaabababbaaaaabbaaaabbbabbbabbbbbbbbaaababbbbabbbbabaabbabbaaa"
                "abaaabbbabbbaabbabaaaababbbbbbbabbbbabbbabaaabaaababbabbaaabbbbbaba",
                2449));
            test.execute(BytesAvailable(""));
            test.execute(BytesAssembled(0));
            test.execute(UnassembledBytes(2629 + 2617));

            // [4910, 6094)
            test.execute(SubmitSegment(
                "aaababaabbbaabaabaabaaabbbabaabababbaaaaabbaaaabbbabbbabbbbbbbbaaababbbbabbbbabaabbabbaaaabaaabbbabbba"
                "abbabaaaababbbbbbbabbbbabbbabaaabaaababbabbaaabbbbbababbbababababaabbabaababbbaababaababbbabbabbabbabb"
                "aaabababaaaaaabaaabbaabbbbabaaabbaabaabbbbaabbaaabbababaababaaabbaabbaabbbbabaaaaababbabaaabbababbbaaa"
                "babbbabbabaabbbbababaaabababaaaabaaabbaaaaaaabbbaabaaabbbabbaaaabababbabaaabaababaababaabbbaaaaabbbabb"
                "baabababbbabaabbaaaababbaabbbaaaabaaabbbbabbbbabbbaaaaababaabbbbaabbabaaabaabbabbbbabaaabaaabbaabbaaaa"
                "aabbbbaabbabbabbaaabbabbabbaababbaaabbbabaaaabbaabaabbbabbaabaaabababbababbaabbbbbbaabbaaaabbbbabaabbb"
                "babaaaaaaabbbbbbabbbbbbbaaaabaaaaaaaabbbbaaababbabaaabbabbaaaabaaabaaabaabaabaabbabbaababbbabbababaaba"
                "ababbaaabaaaaababaababbabbaaababbbabaaaaaaabaabbbbbaaaaabaabaabaabbbababbaabbabaaaabbbaabbbbaababbabba"
                "bbbbbabbbbaabbbaabaaabbaabbbbbbbbbaaabbaaaabababbbaabaabaabbbbbabbaaabbbbababbabababababbabaaabaababba"
                "bbaaabaaaababaabbabbbaabbbababaaaaababababbbbbbaaabbabbbaaaabbbabbbbabaabaabbbababbabbbabbabbaaaababbb"
                "baaabaabaabaaaaaababaaaabaababaaabaababababbaaababbbabaababbbbaabbbaababbbaaabbabbbabbabbbbbbbbbbbaabb"
                "babababaaaaabbabaaabbaaabababbbbbbaabaabbaabaabbaaaaaababaabba",
                4910));
            // expected [2449, 5066) [5066, 6094) [6094, 7982)

            test.execute(BytesAvailable(""));
            test.execute(BytesAssembled(0));
            test.execute(UnassembledBytes(7982 - 2449));

            // [0, 1463)
            test.execute(SubmitSegment(
                "aaaaaaababbaaaababbbababbaababaabaabaaababaabbbbabbbaaaabbbabaabbbbbababbbabbababbabababbbbbbbbabaabaa"
                "baaabbaaabbbbaaaabaaabbbabaabbbbbbaaabaabaaabababbaaaabbaaabbabbbababbaaaabbbbbbbabbaabbbabbbbbaaaabbb"
                "abbbaaabbbaababbbbbbbabaabababbbbabaabaaaabbbababbbbaaabbabaaaaaaaababaabbbaabbaaaabbbabbabbabaababbab"
                "bababaabbbbaabababaaaabbababaabbbbaabbbbbabbbaababaaaaaaaaaaaabbabababaaabaaaabbaabbbbababbabaaabababa"
                "ababbababaabbabaabaabbbabbaaaaabbbbaaabababaaabbbabaaabbbbabbababaabbaaaaaaababaabbabbabaabbabbabbabbb"
                "bbbbaabbbbabbababaabaabbbabaaabbababaabbbbaabaabbbbaaaabababbbabbbaaaabbaaaabababbbbabbbbbaabbbaabaaba"
                "aabbbbbababaabaaabaabbabaaabbaaabbbaaabbaaaaabbbbbbbaabbbbaabaaaaabaaaaabbaababbbababaaabbbbabbabaabbb"
                "bbababaaabaaaaabaaabaaababaababaabbbbaababaabaabaaaaaaabaababbabbabababbabbbaababbbaabaabbbbbabbbaaaba"
                "aababaabaabbaaababaabbbabbabaabababbaabbaaaababababbaaabaaababbbababababababbabbbbbbbbabaaabaaabaabbab"
                "abbbbbabaabbabbaabbaababbabbbaabaaabaaaaaabaabbbbbaabbbaaababbabbaaaababbbaababaabaabbaaabaaabbbbabaab"
                "aaabaaaabaabaabbaabbbbbaabbaaabaaaaabaababbaabbbbbaabaabbbaabbabaabbbbabbbbaaaabbbbaaaaabbaaabbbababaa"
                "abbbbbbabababaaabbabaaaabababbbbbabbabbbbbabbabbbaabbababbaabbbabbbbaaabaaababaaabbbaaabaaaaabbabbabab"
                "aabbaaaabbbbbbabababbabbabbbbaabbaaaabaabbabbbbbaaabaabaaaababaabbabbaabbbbabbaabaaabbabbabaaaaabbbabb"
                "bbbabbaaabbaaabaababbabaabaabaabbaaabaababbbabbbaaaabbbababbbabbaaabbaaabbabbaababababaabaaabbbabbbabb"
                "aaaabbbaaabbbabbaaababaababbaabbaab",
                0));
            test.execute(BytesAvailable(
                "aaaaaaababbaaaababbbababbaababaabaabaaababaabbbbabbbaaaabbbabaabbbbbababbbabbababbabababbbbbbbbabaabaa"
                "baaabbaaabbbbaaaabaaabbbabaabbbbbbaaabaabaaabababbaaaabbaaabbabbbababbaaaabbbbbbbabbaabbbabbbbbaaaabbb"
                "abbbaaabbbaababbbbbbbabaabababbbbabaabaaaabbbababbbbaaabbabaaaaaaaababaabbbaabbaaaabbbabbabbabaababbab"
                "bababaabbbbaabababaaaabbababaabbbbaabbbbbabbbaababaaaaaaaaaaaabbabababaaabaaaabbaabbbbababbabaaabababa"
                "ababbababaabbabaabaabbbabbaaaaabbbbaaabababaaabbbabaaabbbbabbababaabbaaaaaaababaabbabbabaabbabbabbabbb"
                "bbbbaabbbbabbababaabaabbbabaaabbababaabbbbaabaabbbbaaaabababbbabbbaaaabbaaaabababbbbabbbbbaabbbaabaaba"
                "aabbbbbababaabaaabaabbabaaabbaaabbbaaabbaaaaabbbbbbbaabbbbaabaaaaabaaaaabbaababbbababaaabbbbabbabaabbb"
                "bbababaaabaaaaabaaabaaababaababaabbbbaababaabaabaaaaaaabaababbabbabababbabbbaababbbaabaabbbbbabbbaaaba"
                "aababaabaabbaaababaabbbabbabaabababbaabbaaaababababbaaabaaababbbababababababbabbbbbbbbabaaabaaabaabbab"
                "abbbbbabaabbabbaabbaababbabbbaabaaabaaaaaabaabbbbbaabbbaaababbabbaaaababbbaababaabaabbaaabaaabbbbabaab"
                "aaabaaaabaabaabbaabbbbbaabbaaabaaaaabaababbaabbbbbaabaabbbaabbabaabbbbabbbbaaaabbbbaaaaabbaaabbbababaa"
                "abbbbbbabababaaabbabaaaabababbbbbabbabbbbbabbabbbaabbababbaabbbabbbbaaabaaababaaabbbaaabaaaaabbabbabab"
                "aabbaaaabbbbbbabababbabbabbbbaabbaaaabaabbabbbbbaaabaabaaaababaabbabbaabbbbabbaabaaabbabbabaaaaabbbabb"
                "bbbabbaaabbaaabaababbabaabaabaabbaaabaababbbabbbaaaabbbababbbabbaaabbaaabbabbaababababaabaaabbbabbbabb"
                "aaaabbbaaabbbabbaaababaababbaabbaab"));
            test.execute(BytesAssembled(1463));
            test.execute(UnassembledBytes(7982 - 2449));

            // before submit [2449, 5066) [5066, 6094) [6094, 7982)

            // [983, 3,246)
            test.execute(SubmitSegment(
                "aaaababbbaababaabaabbaaabaaabbbbabaabaaabaaaabaabaabbaabbbbbaabbaaabaaaaabaababbaabbbbbaabaabbbaabbaba"
                "abbbbabbbbaaaabbbbaaaaabbaaabbbababaaabbbbbbabababaaabbabaaaabababbbbbabbabbbbbabbabbbaabbababbaabbbab"
                "bbbaaabaaababaaabbbaaabaaaaabbabbababaabbaaaabbbbbbabababbabbabbbbaabbaaaabaabbabbbbbaaabaabaaaababaab"
                "babbaabbbbabbaabaaabbabbabaaaaabbbabbbbbabbaaabbaaabaababbabaabaabaabbaaabaababbbabbbaaaabbbababbbabba"
                "aabbaaabbabbaababababaabaaabbbabbbabbaaaabbbaaabbbabbaaababaababbaabbaabbaaababaaabaabababbabbbbabbbab"
                "abbbaaaaaaababbababaaaaaaaaaabbabaaabaaababaaabaaabaabbaaabbaaaaabbbbaabbababaabaababbbaabbabbaaaaaaab"
                "abbbbbbbaabbbbbaaaabbbbbaaaababbabbbaabbabbbbabaabbaababaaabaabaaaaaaaabbbabbbababbbabbabaabababaaabaa"
                "aabbbabbbaaabbaaabbbbbaaabbabbabaabbabbaababbbaaabaabbaabbbbbbbaaababbabbaaaabaabbabbaabaaaabaaaaabbba"
                "abaabbabbbaababaaaabbbaababbbaabaabbbbaabbabbaaabaabbbaabaaabbabbabbbaabaaabbabaabbaaabbbbabaaaabaaaaa"
                "baaaaabbabbbaaababbbbbbaabbbbbaabababaababbaabaababbbaabababbbaabbbaaabababaaaabaabaabbaaaaaaaabbabbba"
                "aabbaabaaaabbbabaabbaaabbabaaababbbbabbabbaaaaabaaabababbbbabbaaaabbaabbbbbaabbbabaababbbbbaaabaaababa"
                "aaaababbbbbbaabbbbbbabbabaabaabbbabbbabaaaabbbbbababbbabbbababbaababbabbbabbbaababbabaaababbabaaabbbab"
                "baaabbabbbabbbbabbbaababbbabbbbbaabbabaaabbbaabbabbbbbabbaaaabaabbaaaaababbaaaabbbabaaababaaabbabaaabb"
                "bbbaaabbbbbbaaabbbabaababaabbabbbaabbbaaaababbaaaaabbbbbbbaaaaabbaabbbbbaaababbbababbbbbbababaaabbbaaa"
                "aaaaaabbababaaaaabbabbbbaabaaaaabbbbbbbbaabaabbbbaababababaaaaabababababbaabbbaabbaababbbaabbbabbbaaab"
                "abbaabbaabaaaabaabbaabaabbbaabaabbaabbaaaaabbaaabbaaaaaababaabbaaabbbbabaabaababbbabaaaaaabaababbbabbb"
                "baabbabbbabbbabbababaabbaaaaabaaaabaabbaabaaabaaaababbbabbabbababbaaaabbaababbbbbaabbbbababbabaabbabbb"
                "babaababbabaabaaaaaababaaaaaababaabaaababbabababbbbbbbaaaabbbaaaabbbabbaaaabababbbabaabbbbaabbaabaaaba"
                "aabbbbabbbbaaabbbaaabaaabbbbbbbaaaabbbbbaaaabaabaaabbaabbbaabaabbabbbabbbbabbaabbabaabaaabbbbbbaaabbaa"
                "ababbbbaabaaaaabbbbabbbaaaabbbbababbbabaabbaabbabaabaabaaaabbbbbaaaabbababbbabbbabaaaabbaabbbabaaabbbb"
                "bbaaababbaaaabaaabbabaabaabbaaaaabaaababbaababaabbabababbbbaabababaaabbabbbbaabaabaabababaabbbbaabbaab"
                "bbabaaabbababaababbaabbaaaababbaabbaaabbbbbbaaababaaabaabababaaabbbbaaabbbbbaabbbbbbbbaaabaaabaaabaabb"
                "bbabbbbabbabaaabbaa",
                983));

            // after submit

            test.execute(BytesAssembled(7982));
            test.execute(UnassembledBytes(0));
        }

    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return 0;
}