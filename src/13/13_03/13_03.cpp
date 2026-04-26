////////////////////////////////////////////////////////////////////////////////////////////

// chapter : Streams

////////////////////////////////////////////////////////////////////////////////////////////

// section : Iterators

////////////////////////////////////////////////////////////////////////////////////////////

// content : Comment Removal Algorithm

////////////////////////////////////////////////////////////////////////////////////////////

#include <filesystem>
#include <iostream>
#include <iterator>
#include <fstream>
#include <print>
#include <sstream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////

std::string::iterator skip_raw_string(std::string & string, std::string::iterator iterator)
{
    if (iterator == std::end(string))
    {
        return iterator;
    }

    if (*iterator == 'R' && std::next(iterator) != std::end(string) && *std::next(iterator) == '\"')
    {
        auto start_quote = std::next(iterator);

        auto open_bracket = std::next(start_quote);

        while (open_bracket != std::end(string) && *open_bracket != '(')
        {
            ++open_bracket;
        }

        if (open_bracket != std::end(string))
        {
            std::string delimiter(std::next(start_quote), open_bracket);

            std::string terminator = ')' + delimiter + '\"';

            auto pos = string.find(terminator, std::distance(std::begin(string), open_bracket));

            if (pos != -1)
            {
                iterator = std::begin(string) + pos + terminator.size();
            }
        }
    }

    return iterator;
}

////////////////////////////////////////////////////////////////////////////////////////////

void transform(std::string const & path_1, std::string const & path_2)
{
    auto string = (std::stringstream() << std::fstream(path_1, std::ios::in).rdbuf()).str();

    for (auto iterator = std::begin(string); iterator != std::end(string); ++iterator)
    {
        auto next_iterator = skip_raw_string(string, iterator);

        if (next_iterator != iterator)
        {
            iterator = next_iterator;
            continue;
        }

        if (*iterator == '\'')
        {
            do
            {
                ++iterator;
            } 
            while (!(*iterator == '\'' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == '\"')
        {
            do
            {
                ++iterator;
            } 
            while (!(*iterator == '\"' && *std::prev(iterator) != '\\'));
        }
                
        if (*iterator == '/') 
        {
            if (*std::next(iterator) == '/')
            {
                auto end = std::next(iterator, 2);

                while (end != std::end(string) && *end != '\n')
                {
                    ++end;
                }

                iterator = string.erase(iterator, end);
            }
            else if (*std::next(iterator) == '*')
            {
                auto end = std::next(iterator, 3);

                while (!(*end == '/' && *std::prev(end) == '*'))
                {
                    ++end;
                }

                iterator = string.erase(iterator, ++end);
            }
        }

        if (iterator == std::end(string))
        {
            break;
        }
    }

    for (auto iterator = std::begin(string); iterator != std::end(string); )
    {
        auto next_iterator = skip_raw_string(string, iterator);

        if (next_iterator != iterator)
        {
            iterator = next_iterator;
            continue;
        }

        if (*iterator == '\n')
        {
            auto next = std::next(iterator);

            while (next != std::end(string) && (*next == ' ' || *next == '\t' || *next == '\r'))
            {
                ++next;
            }

            if (next != std::end(string) && *next == '\n')
            {
                iterator = string.erase(iterator, next);
                continue;
            }
        }

        ++iterator;
    }

    std::fstream(path_2, std::ios::out) << string;
}

////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto path_1 = "src/13/13_03/source.cpp";

	auto path_2 = "src/13/13_03/output.cpp";

//  ---------------------------------------------------

    transform(path_1, path_2);

//  ---------------------------------------------------

    std::cout << std::format("main : enter char : ");

    std::cin.get();

//  ---------------------------------------------------

    //std::filesystem::remove(path_2);
}

////////////////////////////////////////////////////////////////////////////////////////////
