/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:01:18 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/27 18:53:10 by lcalero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <csignal>
extern volatile sig_atomic_t g_running;

void setupSignals();

#endif // SIGNALS_HPP
