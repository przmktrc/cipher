#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "cli.hpp"
#include "icu.hpp"



namespace cipher::config
{
enum class ErrorHandlingMethod
{
    except,
    report,
    report_on_exit,
    report_only_count_on_exit,
    ignore
} error_handling_method { ErrorHandlingMethod::except };

enum class OperatingMode
{
    unknown,
    from_morse,
    to_morse,
    exchange,
    caesar
} operating_mode { OperatingMode::unknown };

bool verbose { false };

std::string default_morsefile { "morsefile.txt" };
std::vector<std::string> additional_morsefiles {};

UnicodeString exchange_keyphrase {};

int caesar_shift { 0 };
UnicodeString caesar_alphabet { "abcdefghijklmnopqrstuvwxyz" };



void set_error_handling(std::vector<std::string>::iterator& it)
{
    std::string mode = *(++it);

    if (mode == "except")
        error_handling_method = ErrorHandlingMethod::except;
    else if (mode == "report")
        error_handling_method = ErrorHandlingMethod::report;
    else if (mode == "report_on_exit")
        error_handling_method = ErrorHandlingMethod::report_on_exit;
    else if (mode == "report_only_count_on_exit")
        error_handling_method = ErrorHandlingMethod::report_only_count_on_exit;
    else if (mode == "ignore")
        error_handling_method = ErrorHandlingMethod::ignore;
    else
        throw std::runtime_error("Invalid error handling mode. Valid values are: except, report, "
                                 "report_on_exit, report_only_count_on_exit, ignore");
}


void add_morsefile_handler(std::vector<std::string>::iterator& it)
{
    ++it;

    if (*it == "nodefault")
        default_morsefile.clear();
    else
        additional_morsefiles.push_back(*it);
}


void set_exchange_handler(std::vector<std::string>::iterator& it)
{
    operating_mode = OperatingMode::exchange;
    exchange_keyphrase = UnicodeString::fromUTF8(*(++it));
}


void set_caesar_handler(std::vector<std::string>::iterator& it)
{
    operating_mode = OperatingMode::caesar;
    caesar_shift = std::stoi(*(++it));
}


void set_caesar_alphabet_handler(std::vector<std::string>::iterator& it)
{
    caesar_alphabet = UnicodeString::fromUTF8(*(++it));
}


cipher::cli::Parser get_parser()
{
    cipher::cli::Parser parser {};
    parser.set_default_handler(cli::handlers::report_unknown_default_handler)
        .add_handler("--from-morse", [](auto& it) { operating_mode = OperatingMode::from_morse; })
        .add_handler("--to-morse", [](auto& it) { operating_mode = OperatingMode::to_morse; })
        .add_handler("--exchange", set_exchange_handler)
        .add_handler(std::vector { "--on-error", "-e" }, set_error_handling)
        .add_handler(std::vector { "--verbose", "-v" },
                     cipher::cli::handlers::flag_handler(verbose))
        .add_handler("--morsefile", add_morsefile_handler)
        .add_handler("--caesar", set_caesar_handler)
        .add_handler("--caesar-alphabet", set_caesar_alphabet_handler);

    return parser;
}
}
