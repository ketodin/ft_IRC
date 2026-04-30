/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:24:26 by jaubry--          #+#    #+#             */
/*   Updated: 2026/04/30 01:12:15 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include "Server.hpp"
#include "ServerReply.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class ACommand
{
	private:
		ACommand(void);
		ACommand(const ACommand& other);
		ACommand& operator=(const ACommand& other);


	protected:
		static void requireArgsNum(const std::vector<std::string>& args,
								   std::size_t					   num,
								   const std::string&			   usage);
		static void requireArgsNum(const std::vector<std::string>& args,
								   std::size_t					   min,
								   std::size_t					   max,
								   const std::string&			   usage);

		static bool validChannel(const std::string& str);
		static bool validWord(const std::string& str);
		static bool validStr(const std::string& str);
		static bool validMode(const std::string& str);

		static void requireMode(const std::vector<std::string>& args,
								std::size_t						index,
								const std::string&				argName);
		static void requireChannel(const std::vector<std::string>& args,
								   std::size_t					   index,
								   const std::string&			   argName);
		static void requireWord(const std::vector<std::string>& args,
								std::size_t						index,
								const std::string&				argName);
		static void requireStr(const std::vector<std::string>& args,
							   std::size_t					   index,
							   const std::string&			   argName);

		static void optionalWord(const std::vector<std::string>& args,
								 std::size_t					 index,
								 const std::string&				 argName);
		static void optionalParam(const std::vector<std::string>& args,
								  std::size_t					  index,
								  const std::string&			  argName);

		static void requirePosInt(const std::vector<std::string>& args,
								  std::size_t					  index,
								  const std::string&			  argName);

	public:
		~ACommand(void);

#include "ACommandException.inl"

};

#endif // ACOMMAND_HPP
