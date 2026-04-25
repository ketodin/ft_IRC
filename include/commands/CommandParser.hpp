/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:25:09 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/25 21:20:55 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "CommandDispatcher.hpp"
#include <string>
#include <vector>

// Parses a single raw IRC-style std::string and dispatches to
// CommandDispatcher.
//
// Parsing rules (IRC-compliant):
//   1. Leading/trailing whitespace is stripped.
//   2. Tokens are split on one or more spaces.
//   3. The first token is the command name — uppercased before dispatch.
//   4. Trailing argument: if any token begins with ':', everything from that
//      token to the end of the string is one single argument (colon stripped).
//      e.g. "PRIVMSG #chan :hello world" → args = ["#chan", "hello world"]
//   5. Empty lines are silently ignored.
//   6. Unknown commands throw std::runtime_error (propagated from dispatcher).
class CommandParser
{
	private:
		CommandDispatcher& _dispatcher; // non-owning reference

		CommandParser(const CommandParser& other);
		CommandParser& operator=(const CommandParser& other);

		// ── Tokenisation
		// ─────────────────────────────────────────────────────── Split 'line'
		// into [command, arg0, arg1, …] respecting the IRC trailing-argument
		// convention. Returns an empty vector for blank lines.
		static std::vector<std::string> tokenise(const std::string& line);

		// Strip leading and trailing ASCII whitespace from 's'.
		static std::string trim(const std::string& s);

	public:
		// ── Lifecycle
		// ──────────────────────────────────────────────────────────
		explicit CommandParser(CommandDispatcher& dispatcher);
		~CommandParser(void);

		// ── Parsing
		// ──────────────────────────────────────────────────────────── Parse
		// and dispatch one complete IRC line. Throws std::runtime_error on
		// unknown command. Throws std::invalid_argument on malformed arguments
		// (from ACommand validators).
		void parse(Client& client, const std::string& line) const;
};

#endif // COMMAND_PARSER_HPP
