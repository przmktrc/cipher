// A utility file including ICU and providing an {fmt} formatting for icu::UnicodeString

#pragma once

#include <fmt/core.h>
#include <unicode/coll.h>
#include <unicode/schriter.h>
#include <unicode/uchar.h>
#include <unicode/umachine.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>


using icu::UnicodeString;



template<>
struct fmt::formatter<UnicodeString>
{
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator
    {
        if (ctx.begin() != ctx.end() && *(ctx.begin()) != '}')
            throw_format_error("UnicodeString does not take any format specifiers.");
        return ctx.end();
    }

    auto format(UnicodeString const& u_str, format_context& ctx) const -> format_context::iterator
    {
        std::string str;
        u_str.toUTF8String(str);
        return fmt::format_to(ctx.out(), "{}", str);
    }
};
