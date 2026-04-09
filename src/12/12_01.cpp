#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

int main() {
    const double rub_to_usd_rate = 0.0125;
    const char* ru_locale_name = "ru_RU.UTF-8";
    const char* us_locale_name = "en_US.UTF-8";

    std::string input_line;
    std::getline(std::cin, input_line);

    std::stringstream input_stream(input_line);
    try {
        input_stream.imbue(std::locale(ru_locale_name));
    } catch (const std::runtime_error&) {
        std::cerr << "Error: Locale " << ru_locale_name << " is not installed on this system." << std::endl;
        return 1;
    }

    long double amount_kopeks;
    input_stream >> std::get_money(amount_kopeks, true);
    if (input_stream.fail()) {
        std::cerr << "Error: Failed to parse input data. Check if the format matches the locale." << std::endl;
        return 1;
    }

    std::stringstream output_stream;
    try {
        output_stream.imbue(std::locale(us_locale_name));
    } catch (const std::runtime_error&) {
        std::cerr << "Error: Locale " << us_locale_name << " is not installed on this system." << std::endl;
        return 1;
    }

    long double amount_cents = amount_kopeks * rub_to_usd_rate;
    output_stream << std::showbase << std::put_money(amount_cents, false);
    std::cout << output_stream.str() << std::endl;
}
