/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:40:18 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/27 18:43:55 by lcalero          ###   ########.fr       */
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
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
}
