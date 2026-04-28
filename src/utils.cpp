/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 02:46:19 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 02:14:28 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace utils
{
// internal helper not exposed in the header
namespace
{
std::string
escape_char(char ch)
{
	unsigned char c = static_cast<unsigned char>(ch);

	switch (c)
	{
		case ('\n'):
			return ("\\n");
		case '\t':
			return ("\\t");
		case '\r':
			return ("\\r");
		case '\0':
			return ("\\0");
		case '\a':
			return ("\\a");
		case '\b':
			return ("\\b");
		case '\f':
			return ("\\f");
		case '\v':
			return ("\\v");
		case '\\':
			return ("\\\\");
		default:
			break;
	}

	if ((c < 32) || (c > 126))
	{
		std::ostringstream oss;
		oss << "\\x" << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<int>(c);
		return (oss.str());
	}
	return (std::string(1, ch));
}
}

/// Returns str with all non-printable/special bytes escaped like '\n'
std::string
escape_str(const std::string& str)
{
	std::string result;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
		result += escape_char(*it);

	return (result);
}

/// Returns true if every character in str is a decimal digit (0-9).
bool
is_digits(const std::string& str)
{
	return (str.find_first_not_of("0123456789") == std::string::npos);
}

/// Converts a digit-only string to int, throwing on invalid input or overflow.
void
ft_atou(const std::string& str, int& num)
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

std::string
getCurrentTime(void)
{
	time_t		now = time(NULL);
	std::string t	= ctime(&now);
	t.erase(t.find_last_not_of("\n\r") + 1);
	return (t);
}
}
