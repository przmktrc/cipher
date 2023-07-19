#include "cli.hpp"
#include "config.hpp"
#include "icu.hpp"
#include "morse.hpp"
#include "util.hpp"



using namespace cipher;



void prepare_for_selected_mode()
{
    switch (config::operating_mode)
    {
        case config::OperatingMode::from_morse: [[fallthrough]];
        case config::OperatingMode::to_morse:
            if (!config::default_morsefile.empty())
                cipher::morse_table.read_from_file(config::default_morsefile);
            else if (config::additional_morsefiles.empty())
                error::error("Default morsefile disabled and no additional ones specified");

            for (auto const& morsefile : config::additional_morsefiles)
                cipher::morse_table.read_from_file(morsefile);

            if (config::operating_mode == config::OperatingMode::from_morse)
            {
                util::verbose_print("Translating morse to normal...\n");
                cipher::morse_table.sort_for_from();
            }
            else
            {
                util::verbose_print("Translating normal to morse...\n");
                cipher::morse_table.sort_for_to();
            }

            break;
        case config::OperatingMode::unknown: error::error_except("Mode not specified");
        default:
            error::error_except(
                fmt::format("This mode is not handled in prepare_for_selected_mode()"));
    }
}



void run_line_by_line()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (config::operating_mode == config::OperatingMode::to_morse)
            fmt::print("{}\n", to_morse(UnicodeString::fromUTF8(line)));
        else if (config::operating_mode == config::OperatingMode::from_morse)
            fmt::print("{}\n", from_morse(UnicodeString::fromUTF8(line)));
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
