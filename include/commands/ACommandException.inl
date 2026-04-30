/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommandException.inl                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 02:54:46 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMAND_EXCEPTION_INL
#define ACOMMAND_EXCEPTION_INL

class CommandException : public std::runtime_error
{
	public:
		explicit CommandException(const std::string& msg);
		virtual ~CommandException(void) throw();
};

class CommandParamsException : public CommandException
{
	public:
		explicit CommandParamsException(const std::string& cause,
										const std::string& usage);
};

class NeedMoreParamsException : public CommandParamsException
{
	public:
		explicit NeedMoreParamsException(const std::string& usage);
};

class TooManyParamsException : public CommandParamsException
{
	public:
		explicit TooManyParamsException(const std::string& usage);
};

#endif // ACOMMAND_EXCEPTION_INL
