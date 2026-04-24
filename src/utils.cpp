/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 19:24:24 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/24 20:58:44 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

bool
is_digits(const std::string& str)
{
	return (str.find_first_not_of("0123456789") == std::string::npos);
}

void
ft_atou(const std::string& str, int& num) // cppcheck-suppress unusedFunction
{
	if (str.empty() || !is_digits(str))
		throw std::invalid_argument("Invalid input: not all digits or empty");

	std::istringstream iss(str);
	long long		   tmp;

	if (!(iss >> tmp) || !iss.eof())
		throw std::invalid_argument("Conversion failed");

	if (tmp > std::numeric_limits<int>::max())
		throw std::overflow_error("Input overflows int");

	num = static_cast<int>(tmp);
}

static std::string
escape_char(char ch)
{
	unsigned char c = static_cast<unsigned char>(ch);
	switch (c)
	{
		case '\n':
			return "\\n";
		case '\t':
			return "\\t";
		case '\r':
			return "\\r";
		case '\0':
			return "\\0";
		case '\a':
			return "\\a";
		case '\b':
			return "\\b";
		case '\f':
			return "\\f";
		case '\v':
			return "\\v";
		case '\\':
			return "\\\\";
		default:
			break;
	}
	if ((c < 32) || (c > 126))
	{
		std::ostringstream oss;
		oss << "\\x" << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<int>(c);
		return oss.str();
	}
	return (std::string(1, ch));
}

std::string
escape_str(const std::string& str) // cppcheck-suppress unusedFunction
{
	std::string result;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
		result += escape_char(*it);

	return (result);
}
