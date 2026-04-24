/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 19:25:25 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/23 20:09:26 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

bool is_digits(const std::string& str);

void ft_atou(const std::string& str, int& num);

std::string escape_str(const std::string& str);

#endif // UTILS_HPP
