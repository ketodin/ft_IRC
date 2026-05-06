/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:40:18 by lcalero           #+#    #+#             */
/*   Updated: 2026/05/06 18:14:33 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.hpp"

volatile sig_atomic_t g_running = 1;

static void
signalHandler(int signum)
{
	(void)signum;
	g_running = 0;
}

void
setupSignals()
{
	struct sigaction sa;

	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGPIPE, SIG_IGN);
}
