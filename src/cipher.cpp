#include <stdexcept>

#include "cli.hpp"
#include "icu.hpp"
#include "morse.hpp"
#include "util.hpp"



enum class OperatingMode
{
    unknown,
    from_morse,
    to_morse
};



void set_error_handling(std::vector<std::string>::iterator& it)
{
    using namespace cipher::error;

    std::string mode = *(++it);

    if (mode == "except")
        handling_method = HandlingMethod::except;
    else if (mode == "report")
        handling_method = HandlingMethod::report;
    else if (mode == "report_on_exit")
        handling_method = HandlingMethod::report_on_exit;
    else if (mode == "report_only_count_on_exit")
        handling_method = HandlingMethod::report_only_count_on_exit;
    else if (mode == "ignore")
        handling_method = HandlingMethod::ignore;
    else
        error_except("Invalid error handling mode. Valid values are: except, report, "
                     "report_on_exit, report_only_count_on_exit, ignore");
}



int main(int argc, char* argv[])
{
    OperatingMode mode { OperatingMode::unknown };
    cipher::cli::Parser parser {};
    parser.set_default_handler(cipher::cli::handlers::report_unknown_default_handler)
        .add_handler("--from-morse", [&mode](auto& it) { mode = OperatingMode::from_morse; })
        .add_handler("--to-morse", [&mode](auto& it) { mode = OperatingMode::to_morse; })
        .add_handler(std::vector { "--on-error", "-e" }, set_error_handling);
    parser.parse(argc, argv);

    if (mode == OperatingMode::unknown) cipher::error::error_except("Mode not specified");

    if (mode == OperatingMode::from_morse | mode == OperatingMode::to_morse)
        cipher::morse_table.read_from_file("morsefile.txt");

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (mode == OperatingMode::to_morse)
            fmt::print("to_morse: \"{}\"\n", cipher::to_morse(UnicodeString::fromUTF8(line)));
        else if (mode == OperatingMode::from_morse)
            fmt::print("from_morse: \"{}\"\n", cipher::from_morse(UnicodeString::fromUTF8(line)));
    }

    return cipher::error::on_exit();
}
