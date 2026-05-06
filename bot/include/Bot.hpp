/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:36:06 by ekeisler          #+#    #+#             */
/*   Updated: 2026/05/06 02:11:21 by ekeisler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <string>

class Bot
{
	private:
		int			_sockfd;
		std::string _nick;
		std::string _channel;

		void _connect(const std::string& host, int port);
		void _authenticate(const std::string& pass);
		void _loop(void);

		void _send(const std::string& msg) const;
		void _handlePrivmsg(const std::string& raw);

		Bot(void);
		Bot(const Bot& other);
		Bot& operator=(const Bot& other);

	public:
		Bot(const std::string& host,
			int				   port,
			const std::string& pass,
			const std::string& nick);
		~Bot(void);

		void run();
};

#endif
