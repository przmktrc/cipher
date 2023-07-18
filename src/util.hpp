#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "icu.hpp"



namespace cipher::util
{
UChar32 read_whitespace(icu::StringCharacterIterator& it)
{
    UChar32 res = ' ';

    for (UChar32 ch = it.current32(); it.hasNext() && std::isspace(ch); ch = it.next32())
    {
        if (ch == '\n') res = ch;
    }

    if (it.hasNext()) it.previous32();

    return res;
}



void for_each_char32(UnicodeString const& str, auto&& func)
{
    UChar32 ch {};
    for (icu::StringCharacterIterator it { str }; it.hasNext(); it.next32())
    {
        ch = it.current32();
        if (std::isspace(ch)) ch = read_whitespace(it);
        func(ch);
    }
}



void for_each_split(UnicodeString const& str, UChar32 delim, auto&& func)
{
    UnicodeString segment;
    UChar32 ch {};
    for (icu::StringCharacterIterator it { str }; it.hasNext(); it.next32())
    {
        ch = it.current32();
        if (ch == delim)
        {
            func(segment);
            segment.remove();
        }
        else
        {
            segment += ch;
        }
    }
    if (!segment.isEmpty()) func(segment);
}
}



namespace cipher::error
{
enum class HandlingMethod
{
    except,
    report,
    report_on_exit,
    report_only_count_on_exit,
    ignore
} handling_method { HandlingMethod::except };

std::vector<std::string> error_messages {};
uint64_t error_count { 0 };



void error(std::string const& msg)
{
    switch (handling_method)
    {
        case HandlingMethod::except: throw std::runtime_error(msg);
        case HandlingMethod::report:
            ++error_count;
            fmt::print(stderr, "Error: \"{}\". Proceeding...\n", msg);
            break;
        case HandlingMethod::report_on_exit:
            ++error_count;
            error_messages.push_back(msg + '\n');
            break;
        case HandlingMethod::report_only_count_on_exit: ++error_count; break;
        case HandlingMethod::ignore: break;
    }
}



void error_except(std::string const& msg)
{
    throw std::runtime_error(msg);
}



int on_exit()
{
    if (error_count)
    {
        if (handling_method == HandlingMethod::report_only_count_on_exit
            || handling_method == HandlingMethod::report_on_exit)
            fmt::print(stderr, "{} errors reported.\n", error_count);

        if (handling_method == HandlingMethod::report_on_exit)
        {
            fmt::print(stderr, "Reported errors:\n");
            for (auto const& msg : error_messages)
                fmt::print("    {}", msg);
        }
        return 1;
    }
    else
        return 0;
}
}