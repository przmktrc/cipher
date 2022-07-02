#pragma once

#include <string>
#include <string_view>


namespace cipher
{
auto from_morse(std::string_view message) -> std::string;


auto to_morse(std::string_view message) -> std::string;
}
