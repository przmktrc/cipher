#include "caesar.hpp"
#include "cli.hpp"
#include "config.hpp"
#include "exchange.hpp"
#include "icu.hpp"
#include "morse.hpp"
#include "util.hpp"



using namespace cipher;



void prepare_for_selected_mode()
{
    switch (config::operating_mode)
    {
        case config::OperatingMode::from_morse: [[fallthrough]];
        case config::OperatingMode::to_morse: cipher::prepare_for_morse(); break;
        case config::OperatingMode::exchange: cipher::prepare_for_exchange(); break;
        case config::OperatingMode::caesar: cipher::prepare_for_caesar(); break;
        case config::OperatingMode::unknown: error::error_except("Mode not specified");
        default:
            error::error_except(
                fmt::format("This mode is not handled in prepare_for_selected_mode()"));
    }
}



void run_line_by_line()
{
    std::string line;

    const std::function<UnicodeString(UnicodeString)> selected_cipher {
        []()
        {
            switch (config::operating_mode)
            {
                case config::OperatingMode::to_morse: return cipher::to_morse;
                case config::OperatingMode::from_morse: return cipher::from_morse;
                case config::OperatingMode::exchange: return cipher::exchange;
                case config::OperatingMode::caesar: return cipher::caesar;
                default: error::error_except("This mode is not handled in run_line_by_line()");
            }
        }()
    };

    while (std::getline(std::cin, line))
    {
        fmt::print("{}\n", selected_cipher(UnicodeString::fromUTF8(line)));
    }
}



int main(int argc, char* argv[])
{
    cli::Parser parser = config::get_parser();
    parser.parse(argc, argv);

    prepare_for_selected_mode();

    run_line_by_line();

    return error::on_exit();
}
