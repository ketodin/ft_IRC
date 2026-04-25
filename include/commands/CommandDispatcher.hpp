/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:24:50 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/25 21:22:31 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include "NickCommand.hpp"
#include "PassCommand.hpp"
#include "UserCommand.hpp"

#include "JoinCommand.hpp"
#include "PrivmsgCommand.hpp"

#include "InviteCommand.hpp"
#include "KickCommand.hpp"
#include "ModeCommand.hpp"
#include "TopicCommand.hpp"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

typedef void (*CommandHandler)(Client&						   client,
							   const std::vector<std::string>& args);

// Maps uppercased command names to their handler function pointer.
// Owns nothing — all pointers are to static methods with static storage.
class CommandDispatcher
{
	private:
		// ── Internal map
		// ───────────────────────────────────────────────────────
		typedef std::map<std::string, CommandHandler> HandlerMap;
		HandlerMap									  _handlers;

		// ── Non-copyable
		// ───────────────────────────────────────────────────────
		CommandDispatcher(const CommandDispatcher& other);
		CommandDispatcher& operator=(const CommandDispatcher& other);

		static void displayCommand(const std::string&			   name,
								   const std::vector<std::string>& args);

	public:
		// ── Lifecycle
		// ──────────────────────────────────────────────────────────
		CommandDispatcher(void);
		~CommandDispatcher(void);

		// ── Registration
		// ─────────────────────────────────────────────────────── Register a
		// handler for 'name' (must be uppercase). Overwrites silently if 'name'
		// is already registered.
		void registerCommand(const std::string& name, CommandHandler handler);

		// ── Dispatch
		// ─────────────────────────────────────────────────────────── Call the
		// handler registered under 'name' with 'args'. Throws
		// std::runtime_error if 'name' is not registered.
		void dispatch(Client&						  client,
					  const std::string&			  name,
					  const std::vector<std::string>& args) const;

		/*
		// ── Introspection
		// ────────────────────────────────────────────────────── Returns true
		// if a handler is registered under 'name'.
		bool hasCommand(const std::string& name) const;

		// Returns the number of registered commands.
		std::size_t commandCount(void) const;
		*/
};

#endif // COMMAND_DISPATCHER_HPP
