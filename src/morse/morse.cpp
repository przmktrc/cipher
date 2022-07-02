#include "cipher/morse.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <iostream>
#include <optional>
#include <ranges>



struct morse_alphabet_t
{
private:
    std::vector<std::pair<std::string_view, std::string_view>> const m_alphabet {
        { " ", "/" },      { "a", ".-" },     { "b", "-..." },   { "c", "-.-." },    { "d", "-.." },
        { "e", "." },      { "f", "..-." },   { "g", "--." },    { "h", "...." },    { "i", ".." },
        { "j", ".---" },   { "k", "-.-" },    { "l", ".-.." },   { "m", "--" },      { "n", "-." },
        { "o", "---" },    { "p", ".--." },   { "q", "--.-" },   { "r", ".-." },     { "s", "..." },
        { "t", "-" },      { "u", "..-" },    { "v", "...-" },   { "w", ".--" },     { "x", "-..-" },
        { "y", "-.--" },   { "z", "--.." },   { "0", "-----" },  { "1", ".----" },   { "2", "..---" },
        { "3", "...--" },  { "4", "....-" },  { "5", "....." },  { "6", "-...." },   { "7", "--..." },
        { "8", "---.." },  { "9", "----." },  { "ą", ".-.-" },   { "ć", "-.-.." },   { "ę", "..-.." },
        { "ł", ".-..-" },  { "ń", "--.--" },  { "ó", "---." },   { "ś", "...-..." }, { "ż", "--..-." },
        { "ź", "--..-" },  { ".", ".-.-.-" }, { ",", "--..--" }, { "'", ".----." },  { "\"", ".-..-." },
        { "_", "..--.-" }, { ":", "---..." }, { ";", "-.-.-." }, { "?", "..--.." },  { "!", "-.-.--" },
        { "-", "-....-" }, { "+", ".-.-." },  { "/", "-..-." },  { "(", "-.--." },   { ")", "-.--.-" },
        { "=", "-...-" },  { "@", ".--.-." }
    };

public:
    auto from_morse(std::string_view letter) const -> std::optional<std::string_view>
    {
        auto it = std::ranges::find_if(m_alphabet, [letter](auto const& x) -> bool { return x.second == letter; });

        return (it == m_alphabet.end() ? std::nullopt : std::make_optional(it->first));
    }

    auto to_morse(std::string_view letter) const -> std::optional<std::string_view>
    {
        auto it = std::ranges::find_if(m_alphabet, [letter](auto const& x) -> bool { return x.first == letter; });

        return (it == m_alphabet.end() ? std::nullopt : std::make_optional(it->second));
    }
} const morse_alphabet {};



auto to_string(std::ranges::input_range auto const& range) -> std::string
{
    if (range.begin() == range.end()) return "/";

    auto string = std::string {};

    for (auto const& elem : range)
    {
        string += elem;
    }

    return string;
}



auto cipher::from_morse(std::string_view message) -> std::string
{
    auto result = std::string {};

    for (auto const& letter : message | std::views::split('/'))
    {
        auto letter_str = to_string(letter);

        if (auto letter_res = morse_alphabet.from_morse(letter_str))
            result += *letter_res;
        else
            result += fmt::format("/{}/", letter_str);
    }

    return result;
}
