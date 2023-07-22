#pragma once

#include <cstdint>
#include <unordered_map>

#include "config.hpp"
#include "icu.hpp"
#include "util.hpp"



namespace cipher
{
class ExchangeTable
{
public:
    void set_keyphrase(UnicodeString keyphrase)
    {
        verify_keyphrase_length(keyphrase);

        keyphrase.toLower();

        for (uint64_t i { 0 }; i < keyphrase.countChar32() - 1; i += 2)
        {
            if (table.contains(keyphrase.char32At(i)))
            {
                error::error(fmt::format("Duplicate letter in keyphrase: \"{}\"",
                                         UnicodeString(keyphrase.char32At(i))));
                continue;
            }
            if (table.contains(keyphrase.char32At(i + 1)))
            {
                error::error(fmt::format("Duplicate letter in keyphrase: \"{}\"",
                                         UnicodeString(keyphrase.char32At(i))));
                continue;
            }

            table[keyphrase.char32At(i)] = keyphrase.char32At(i + 1);
            table[keyphrase.char32At(i + 1)] = keyphrase.char32At(i);
        }
    }


    UChar32 exchange(UChar32 ch) const
    {
        bool is_upper { util::isupper(ch) };

        if (is_upper) ch = util::tolower(ch);

        if (table.contains(ch)) ch = table.at(ch);

        return (is_upper ? util::toupper(ch) : ch);
    }


private:
    void verify_keyphrase_length(UnicodeString const& keyphrase)
    {
        if (keyphrase.countChar32() % 2 != 0)
        {
            if (config::error_handling_method == config::ErrorHandlingMethod::except)
                error::error(fmt::format(
                    "Exchange keyphrase \"{}\" has an odd number of characters.", keyphrase));
            else
                error::error(fmt::format("Exchange keyphrase \"{}\" has an odd number of "
                                         "characters. Ignoring the last character.",
                                         keyphrase));
        }
    }


    std::unordered_map<UChar32, UChar32> table;
} exchange_table {};



void prepare_for_exchange()
{
    cipher::exchange_table.set_keyphrase(config::exchange_keyphrase);
    util::verbose_print(fmt::format("Running exchange cipher with keyphrase \"{}\"...\n",
                                    config::exchange_keyphrase));
}



UnicodeString exchange(UnicodeString msg)
{
    msg.trim();

    UnicodeString res;

    util::for_each_char32(msg, [&res](UChar32 ch) { res += exchange_table.exchange(ch); });

    return res;
}
}
