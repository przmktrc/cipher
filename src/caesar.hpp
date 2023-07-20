#pragma once

#include <cstdint>
#include <fstream>
#include <iterator>

#include "config.hpp"
#include "icu.hpp"
#include "util.hpp"



namespace cipher
{
UChar32 caesar_letter(UChar32 ch)
{
    int64_t idx { static_cast<int64_t>(util::find_char_position(config::caesar_alphabet, ch)) };

    if (idx == config::caesar_alphabet.countChar32()) return ch;

    idx = (idx + config::caesar_shift) % config::caesar_alphabet.countChar32();
    while (idx < 0)
        idx += config::caesar_alphabet.countChar32();

    return config::caesar_alphabet[idx];
}



UnicodeString caesar(UnicodeString msg)
{
    msg.trim();

    UnicodeString res;

    util::for_each_char32(msg, [&res](UChar32 ch) { res += caesar_letter(ch); });

    return res;
}
};
