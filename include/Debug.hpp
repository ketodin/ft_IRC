/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 18:40:41 by lcalero           #+#    #+#             */
/*   Updated: 2026/05/08 18:50:31 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifndef DEBUG
#	define DEBUG 0
#endif

#include <iostream>
#include <string>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define GREEN "\033[32m"
#define RED "\033[31m"

#define DBG_SEPARATOR                                                          \
	std::cout << DIM "─────────────────────────────────────────" RESET "\n"

#define DBG_SECTION(label) std::cout << BOLD CYAN "[ " label " ]" RESET "\n"

#define DBG_FIELD(key, value)                                                  \
	std::cout << "  " YELLOW key RESET ": " << (value) << "\n"

#define DBG_ARG(index, value)                                                  \
	std::cout << "  " MAGENTA "arg" << (index) << RESET ": `" << (value)       \
			  << "`\n"

#define DBG_CMD(name)                                                          \
	std::cout << BOLD GREEN "cmd" RESET ": [" << (name) << "]\n"

#define DBG_WARN(msg) std::cout << BOLD RED "warn" RESET ": " << (msg) << "\n"

#define DBG_REPLY(msg)                                                         \
	std::cout << BOLD CYAN ">>>" RESET " " DIM << (msg) << RESET "\n"

#endif // DEBUG_HPP
