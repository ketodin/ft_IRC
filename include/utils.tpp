/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 02:45:08 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/25 02:51:18 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_TPP
#define UTILS_TPP

namespace utils
{
// Converts any streamable value to its string representation.
template<typename T>
std::string
toString(T val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

/*
	Functor that calls a const getter on a pointer-to-object and compares
	the result to an expected value.

	Usage:
		std::find_if(v.begin(), v.end(),
			utils::HasMemberValue<Client, int>(&Client::getFd, fd));
*/
template<typename ClassType, typename ReturnType>
HasMemberValue<ClassType, ReturnType>::HasMemberValue(Getter	 getter,
													  ReturnType expected) :
	_getter(getter),
	_expected(expected)
{
}

template<typename ClassType, typename ReturnType>
bool
HasMemberValue<ClassType, ReturnType>::operator()(const ClassType* obj) const
{
	return ((obj->*_getter)() == _expected);
}

}

#endif // UTILS_TPP
