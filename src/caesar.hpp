#pragma once

#include <cstdint>
#include <fstream>
#include <iterator>

#include "config.hpp"
#include "icu.hpp"
#include "util.hpp"



namespace cipher
{
UnicodeString read_caesar_alphabet_file(std::string const& filename)
{
    util::verbose_print(
        fmt::format("Reading Caesar's cipher alphabet from file \"{}\"...\n", filename));

    UnicodeString res;
    std::ifstream file { filename };

    if (file.bad() || !file.is_open())
    {
        error::error(
            fmt::format("Couldn't read Caesar's cipher alphabet from file \"{}\"", filename));
        return config::caesar_alphabet;
    }

    file >> res;

    if (res.isEmpty())
    {
        if (config::error_handling_method == config::ErrorHandlingMethod::except)
            error::error(fmt::format("Alphabet read from file \"{}\" is empty", filename));
        else
            error::error(fmt::format(
                "Alphabet read from file \"{}\" is empty. Proceeding with the default alphabet",
                filename));
        return config::caesar_alphabet;
    }

    return res;
}



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
