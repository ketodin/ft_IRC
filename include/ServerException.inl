/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.inl                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:27:59 by lcalero           #+#    #+#             */
/*   Updated: 2026/04/25 02:54:46 by jaubry--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_EXCEPTION_INL
#define SERVER_EXCEPTION_INL

class ServerException : public std::runtime_error
{
	public:
		explicit ServerException(const std::string& msg);
		virtual ~ServerException() throw();
};

class EpollCreateException : public ServerException
{
	public:
		explicit EpollCreateException();
};

class FcntlException : public ServerException
{
	public:
		explicit FcntlException(const std::string& flag);
};

class EpollCtlException : public ServerException
{
	public:
		explicit EpollCtlException(const std::string& op);
};

class EpollWaitException : public ServerException
{
	public:
		explicit EpollWaitException();
};

class CreateSocketException : public ServerException
{
	public:
		explicit CreateSocketException();
};

class SocketOptException : public ServerException
{
	public:
		explicit SocketOptException(const std::string& opt);
};

class BindException : public ServerException
{
	public:
		explicit BindException(int port);
};

class ListenException : public ServerException
{
	public:
		explicit ListenException(int port);
};

class AcceptException : public ServerException
{
	public:
		explicit AcceptException();
};

class PortNumberException : public ServerException
{
	public:
		explicit PortNumberException(const std::string& port);
};

class InvalidPortRangeException : public ServerException
{
	public:
		explicit InvalidPortRangeException(const std::string& port);
};

#endif // SERVER_EXCEPTION_INL
