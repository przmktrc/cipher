#pragma once

#include <algorithm>
#include <fstream>
#include <optional>
#include <vector>

#include "icu.hpp"
#include "util.hpp"



namespace cipher
{
class MorseTable
{
public:
    void read_from_file(std::string const& filename)
    {
        util::verbose_print(fmt::format("Reading morse file \"{}\"...\n", filename));

        std::ifstream file { filename };

        if (file.bad() || !file.is_open())
            error::error(fmt::format("Could not load morse file \"{}\"", filename));

        for (UnicodeString normal, morse; file >> normal >> morse;)
        {
            table.emplace_back(normal, morse);
        }
    }


    void sort_for_to()
    {
        std::sort(table.begin(), table.end(), compare_normal);
    }

    void sort_for_from()
    {
        std::sort(table.begin(), table.end(), compare_morse);
    }


    std::optional<UnicodeString> to(UnicodeString const& to) const
    {
        auto it
            = std::lower_bound(table.begin(), table.end(), ExchangePair { to, "" }, compare_normal);

        if (it != table.end() && it->first == to)
            return it->second;
        else
            return std::nullopt;
    }

    std::optional<UnicodeString> from(UnicodeString const& from) const
    {
        auto it = std::lower_bound(
            table.begin(), table.end(), ExchangePair { "", from }, compare_morse);

        if (it->second == from)
            return it->first;
        else
            return std::nullopt;
    }

private:
    // Format: {normal_symbol, morse_symbol}
    using ExchangePair = std::pair<UnicodeString, UnicodeString>;

    std::vector<ExchangePair> table {};

    static bool compare_normal(ExchangePair const& lhs, ExchangePair const& rhs)
    {
        return lhs.first < rhs.first;
    }

    static bool compare_morse(ExchangePair const& lhs, ExchangePair const& rhs)
    {
        return lhs.second < rhs.second;
    }
} morse_table;



UnicodeString to_morse(UnicodeString msg)
{
    msg.toLower();
    msg.trim();

    UnicodeString res;

    morse_table.sort_for_to();

    util::for_each_char32(
        msg,
        [&res](UChar32 ch) -> void
        {
            if (ch == ' ')
                res += '/';
            else if (ch == '\n')
                res += '\n';
            else
            {
                auto morse = morse_table.to(ch);
                if (!morse.has_value())
                {
                    error::error(fmt::format(
                        "Could not find morse symbol for \"{}\" ({})", UnicodeString(ch), ch));
                }
                else
                {
                    res += morse.value();
                    res += '/';
                }
            }
        });

    return res;
}



UnicodeString from_morse(UnicodeString msg)
{
    msg.trim();

    UnicodeString res;

    morse_table.sort_for_from();

    util::for_each_split(msg,
                         '/',
                         [&res](UnicodeString symbol) -> void
                         {
                             if (symbol == "")
                             {
                                 res += ' ';
                                 return;
                             }
                             if (symbol.startsWith('\n'))
                             {
                                 res += '\n';
                                 symbol.trim();
                             }

                             auto ch = morse_table.from(symbol);

                             if (!ch.has_value())
                             {
                                 error::error(
                                     fmt::format("Could not find morse symbol \"{}\"", symbol));
                             }
                             else
                             {
                                 res += ch.value();
                             }
                         });

    return res;
}
}
