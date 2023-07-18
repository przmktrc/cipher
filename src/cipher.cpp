#include "cli.hpp"
#include "config.hpp"
#include "icu.hpp"
#include "morse.hpp"
#include "util.hpp"



using namespace cipher;



void prepare_for_selected_mode()
{
    if (config::operating_mode == config::OperatingMode::unknown)
        cipher::error::error_except("Mode not specified");

    if (config::operating_mode == config::OperatingMode::from_morse
        | config::operating_mode == config::OperatingMode::to_morse)
        cipher::morse_table.read_from_file("morsefile.txt");
}



void run_line_by_line()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (config::operating_mode == config::OperatingMode::to_morse)
            fmt::print("to_morse: \"{}\"\n", to_morse(UnicodeString::fromUTF8(line)));
        else if (config::operating_mode == config::OperatingMode::from_morse)
            fmt::print("from_morse: \"{}\"\n", from_morse(UnicodeString::fromUTF8(line)));
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
