/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotSignals.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 15:44:23 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 16:32:41 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BotSignals.hpp"

volatile sig_atomic_t g_bot_running = 1;

static void
signalHandler(int signum)
{
	(void)signum;
	g_bot_running = 0;
}

void
setupSignals(void)
{
	struct sigaction sa;

	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	signal(SIGPIPE, SIG_IGN);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}
