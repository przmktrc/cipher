#include <iostream>
#include <string>
#include <sys/types.h>
#include <unicode/schriter.h>
#include <unicode/unistr.h>

#include "fmt/core.h"
#include "icu.hpp"



UnicodeString display_char(UChar32 ch)
{
    switch (ch)
    {
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        default: return UnicodeString(ch);
    }
}



void analyze_char(UChar32 ch)
{
    fmt::print("   > character \"{}\" ({})\n", display_char(ch), int(ch));
}



void analyze_line(UnicodeString const& line)
{
    fmt::print(" > length() {} countChar32() {}\n", line.length(), line.countChar32());

    for (icu::StringCharacterIterator it { line }; it.hasNext(); it.next32())
    {
        analyze_char(it.current32());
    }
}



int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        analyze_line(icu::UnicodeString::fromUTF8(line));
    }
}
