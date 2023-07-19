#pragma once

#include <stdexcept>
#include <vector>

#include "cli.hpp"



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
    to_morse
} operating_mode { OperatingMode::unknown };

bool verbose { false };

std::string default_morsefile { "morsefile.txt" };
std::vector<std::string> additional_morsefiles {};



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


cipher::cli::Parser get_parser()
{
    cipher::cli::Parser parser {};
    parser.set_default_handler(cipher::cli::handlers::report_unknown_default_handler)
        .add_handler("--from-morse", [](auto& it) { operating_mode = OperatingMode::from_morse; })
        .add_handler("--to-morse", [](auto& it) { operating_mode = OperatingMode::to_morse; })
        .add_handler(std::vector { "--on-error", "-e" }, set_error_handling)
        .add_handler(std::vector { "--verbose", "-v" },
                     cipher::cli::handlers::flag_handler(verbose))
        .add_handler("--morsefile", add_morsefile_handler);

    return parser;
}
}
