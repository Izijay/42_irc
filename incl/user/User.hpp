#ifndef USER_HPP
#define USER_HPP

#define BUFFER_SIZE 4096

#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ctime>

class Server;
class Channel;

class User
{
	public:
		User(int fd, struct sockaddr_in, Server *server);
		~User();

		void			receiveMessage(void);
		void			sendReply(int, std::string const, std::vector<std::string> const, Channel *);


		int							getFd(void) const;
		bool						getTransfer(void) const;
		bool						getPutUnavailable(void) const;
		bool						getTryAgain(void) const;
		std::time_t					getLastPing(void) const;
		std::time_t					getFailPing(void) const;
		bool						getTimeout(void) const;
		std::string					getNickname(void) const;
		std::string					getUsername(void) const;
		std::string					getRealname(void) const;
		std::string					getHostaddr(void) const;
		std::string					getHostname(void) const;
		std::string					getMode(void) const;
		std::string					getPrefix(void) const;
		std::vector<std::string>	getCommandMessage(void) const;
		std::map<std::string, void (*)(std::string &, User *, Server *)>	getCommandFunction(void) const;


		void			setNickname(std::string const &nick);
		void			setUsername(std::string const &user);
		void			setRealname(std::string const &realname);
		void			setHostaddr(std::string const &hostaddr);
		void			setHostname(std::string const &hostname);
		void			setPrefix(void);
		void			setMode(std::string const mode, bool oper);
		void			setLastPing();
		void			setFailPing();
		void			setTimeout(bool);
		void			setPwd(bool);
		void			setTransfer(bool change);
		void			setPutUnavailable(bool change);
		void			setTryAgain(bool);


	private:
		void			parseMessage(void);

	private:
		int			_fd;
		Server		*_server;
		std::time_t	_last_ping;
		std::time_t	_fail_ping;
		bool		_timeout;
		bool		_pwd;

		std::string	_buffer;
		std::vector<std::string>	_commands_message;

		std::map<std::string, void (*)(std::string &, User *, Server *)>	_commands_function;
		std::string	_prefix;

		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string _hostaddr;
		std::string	_hostname;

		std::string	_mode;

		bool		_transfer;
		bool		_put_unavailable;
		bool		_try_again;

};

#endif
