#pragma once

#include <cstdint>
#include <fmt/core.h>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>



namespace cipher::cli
{
namespace handlers
{
void empty_handler(std::vector<std::string>::iterator& it) {}


// Saves the next arg into `value`
struct set_string_handler
{
    std::string& value;

    explicit set_string_handler(std::string& value) : value(value) {}

    void operator()(std::vector<std::string>::iterator& it)
    {
        value = *(++it);
    }
};


// Saves the current arg into `value`
struct set_string_defualt_handler
{
    std::string& value;

    explicit set_string_defualt_handler(std::string& value) : value(value) {}

    void operator()(std::vector<std::string>::iterator& it)
    {
        value = *it;
    }
};


// Sets `value` to `true`
struct flag_handler
{
    bool& value;

    explicit flag_handler(bool& value) : value(value) {}

    void operator()(std::vector<std::string>::iterator& it)
    {
        value = true;
    }
};


// Saves the next arg into `value` as an `int`
struct set_int_handler
{
    int& value;

    explicit set_int_handler(int& value) : value(value) {}

    void operator()(std::vector<std::string>::iterator& it)
    {
        value = std::stoi(*(++it));
    }
};


// Reports an unknown option
void report_unknown_default_handler(std::vector<std::string>::iterator& it)
{
    throw std::runtime_error(fmt::format("Unknown option \"{}\"", *it));
}
}



class Parser
{
public:
    using Handler = std::function<void(std::vector<std::string>::iterator&)>;


    Parser& add_handler(std::string const& option, Handler const& handler)
    {
        handlers[option] = handler;
        return *this;
    }

    Parser& add_handler(std::ranges::input_range auto options, Handler const& handler)
    {
        for (std::string const& option : options)
        {
            handlers[option] = handler;
        }
        return *this;
    }


    Parser& set_default_handler(Handler const& handler)
    {
        default_handler = handler;
        return *this;
    }


    void parse(int argc, char* argv[])
    {
        auto args = vectorize_argvs(argc, argv);

        for (auto it = args.begin(); it != args.end(); ++it)
        {
            if (handlers.contains(*it))
                handlers[*it](it);
            else
                default_handler(it);
        }
    }


private:
    std::vector<std::string> vectorize_argvs(int argc, char* argv[])
    {
        std::vector<std::string> res(argc - 1);

        for (uint64_t i = 1; i < argc; ++i)
        {
            res[i - 1] = argv[i];
        }

        return res;
    }

    Handler default_handler { handlers::empty_handler };
    std::map<std::string, Handler> handlers;
};
}
