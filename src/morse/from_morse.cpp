#include <fmt/core.h>
#include <iostream>
#include <string>

#include "cipher/morse.hpp"



auto concat_argvs(int argc, char* argv[]) -> std::string
{
    auto result = std::string { argv[1] };

    for (uint64_t i = 2; i < argc; ++i)
    {
        result += ' ';
        result += argv[i];
    }

    return result;
}



auto main(int argc, char* argv[]) -> int
{
    if (argc > 1) { fmt::print("{}\n", cipher::from_morse(concat_argvs(argc, argv))); }
    else
    {
        auto line = std::string {};

        while (std::getline(std::cin, line))
        {
            fmt::print("{}\n", cipher::from_morse(line));
        }
    }
}
