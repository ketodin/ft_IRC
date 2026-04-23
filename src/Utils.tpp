/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 22:47:52 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/23 22:48:43 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

template<typename T>
std::string
toString(T val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}
