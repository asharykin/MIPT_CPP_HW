#include <cctype>
#include <iostream>
#include <istream>
#include <print>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <cmath>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
public :

	using token_t = std::variant < char, double, std::string > ;

//  ------------------------------------------------------------------------------------

	Stream(std::string const & string) : m_stream(string + ';') { }

//  ------------------------------------------------------------------------------------

	auto empty()
	{
		return m_stream.peek() == ';';
	}

//  ------------------------------------------------------------------------------------

	auto get()
	{
		if (m_has_token)
		{
			m_has_token = false;

			return m_token;
		}

		auto x = '\0';

		m_stream >> x;

		switch (x)
		{
			case '+' : case '-' : case '*' : case '/' : case '(' : case ')' : case ';' :
			case '%' : case '^' : case '!' : case '[' : case ']' : case '{' : case '}' :
			{
				return token_t(x);
			}
			case '0' : case '1' : case '2' : case '3' : case '4' :
			case '5' : case '6' : case '7' : case '8' : case '9' :
			case '.' :
			{
				m_stream.unget();

				auto y = 0.0;

				m_stream >> y;

				return token_t(y);
			}
			default :
			{
				std::string string(1, x);

				while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
				{
					string += x;
				}

				if (!std::isspace(x))
				{
					m_stream.unget();
				}

				return token_t(string);
			}
		}
	}

//  ------------------------------------------------------------------------------------

	void put(token_t const & token)
	{
		m_token = token;

		m_has_token = true;
	}

private :

	std::stringstream m_stream;

	token_t m_token;

	bool m_has_token = false;
};

///////////////////////////////////////////////////////////////////////////////////////////

class Calculator
{
public :

	void test(const std::string& path)
	{
		std::ifstream file(path);

		std::string string;

		while (std::getline(file, string))
		{
			if (Stream stream(string); !stream.empty())
			{
				std::cout << std::format("Calculator::test : {} = {}\n", string, statement(stream));
			}
			else
			{
				break;
			}
		}
	}

private :

	auto statement(Stream & stream) -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < std::string > (token))
		{
			if (std::get < std::string > (token) == "set")
			{
				return declaration(stream);
			}
		}

		stream.put(token);

		return expression(stream);
	}

//  ----------------------------------------------------------------------------------

	auto declaration(Stream & stream) -> double
	{
		auto string = std::get < std::string > (stream.get());

		m_variables[string] = expression(stream);

		return m_variables[string];
	}

//  ----------------------------------------------------------------------------------

	auto expression(Stream & stream) const -> double
	{
		auto x = term(stream);

		auto token = stream.get();

		while (true)
		{
			switch (std::get < char > (token))
			{
				case '+' : { x += term(stream); break; }

				case '-' : { x -= term(stream); break; }

				default  :
				{
					stream.put(token);

					return x;
				}
			}

			token = stream.get();
		}
	}

//  ----------------------------------------------------------------------------------

	auto term(Stream & stream) const -> double
	{
		auto x = power(stream);

		auto token = stream.get();

		while (true)
		{
			switch (std::get < char > (token))
			{
				case '*' : { x *= power(stream); break; }

				case '/' : { x /= power(stream); break; }

				case '%' : { x = std::fmod(x, power(stream)); break; }

				default  :
				{
					stream.put(token);

					return x;
				}
			}

			token = stream.get();
		}
	}

//  ----------------------------------------------------------------------------------

	auto power(Stream & stream) const -> double
	{
		auto x = suffix(stream);

		auto token = stream.get();

		while (true)
		{
			switch (std::get < char > (token))
			{
				case '^' : { x = std::pow(x, suffix(stream)); break; }

				default  :
				{
					stream.put(token);

					return x;
				}
			}

			token = stream.get();
		}
	}

//  ----------------------------------------------------------------------------------

	auto suffix(Stream & stream) const -> double
	{
		auto x = primary(stream);

		auto token = stream.get();

		while (true)
		{
			switch (std::get < char > (token))
			{
				case '!' :
				{
					auto res = 1.0;

					for (auto i = 1; i <= x; ++i) {
						res *= i;
					}

					x = res;

					break;
				}
				default  :
				{
					stream.put(token);

					return x;
				}
			}

			token = stream.get();
		}
	}

//  ----------------------------------------------------------------------------------

	auto primary(Stream & stream) const -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < char > (token))
		{
			switch (std::get < char > (token))
			{
				case '(' : case '[' : case '{' :
				{
					auto x = expression(stream);

					stream.get();

					return x;
				}
				case '+' : { return      primary(stream); }

				case '-' : { return -1 * primary(stream); }
			}
		}

		if (std::holds_alternative < double > (token))
		{
			return std::get < double > (token);
		}

		return m_variables.at(std::get < std::string > (token));
	}

//  ----------------------------------------------------------------------------------

	std::unordered_map < std::string, double > m_variables;
};

///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Calculator().test("src/13/13_02/input.txt");
}

///////////////////////////////////////////////////////////////////////////////////////////
