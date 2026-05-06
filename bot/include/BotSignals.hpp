/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotSignals.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 15:45:09 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 16:32:26 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTSIGNALS_HPP
#define BOTSIGNALS_HPP

#include <csignal>
extern volatile sig_atomic_t g_bot_running;

void setupSignals();

#endif // SIGNALS_HPP
