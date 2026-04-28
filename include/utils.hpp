/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 02:44:21 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/28 17:03:55 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace utils
{
bool is_digits(const std::string& str);

void ft_atou(const std::string& str, int& num);

std::string escape_str(const std::string& str);

template<typename T>
std::string toString(T val);

template<typename ClassType, typename ReturnType>
struct HasMemberValue
{
		typedef ReturnType (ClassType::*Getter)() const;

		Getter	   _getter;
		ReturnType _expected;

		HasMemberValue(Getter getter, ReturnType expected);
		bool operator()(const ClassType* obj) const;
};

std::string getCurrentTime(void);

}

#include "utils.tpp"

#endif // UTILS_HPP
