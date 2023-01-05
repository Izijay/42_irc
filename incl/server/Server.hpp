#ifndef SERVER_HPP
#define SERVER_HPP

#include "../user/User.hpp"
#include "../channel/Channel.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <exception>

#define MAX_USER 500
#define USER_TIMEOUT 300
#define TIME_AUTO_MESSAGE 300

class User;
class Channel;

class Server
{
	public:
		Server(char *port, char *pwd, bool *exec);
		~Server();

		void	init();
		void	exec();
		void	connectUser();
		void	delUser(User*, std::string const);
		void	delChannel(Channel*);

		void	error(std::string, bool);

		std::map<int, User*>			getUser(void) const;
		std::map<std::string, Channel*>	getChannel(void) const;
		std::vector<std::string>		getUnavaliableNick(void) const;
		std::string const				getCreatedDate(void) const;
		std::string const				getPwd(void) const;
		std::string const				getServerName(void) const;
		std::string const				getVersion(void) const;
		std::string const				getChannelMode(void) const;
		std::string const				getUserMode(void) const;
		std::vector<std::string>		getConf(void) const;
		std::vector<std::string>		getMotd(void) const;
		int								getUserFd(std::string const) const;

		std::vector<Channel*>			listChannelByUser(int fd) const;
		void							putUnavailableNick(std::string &nick);

		void							setChannel(std::string const, std::string const, User*);
		void							setExec(bool);

		void							autoMessage();

		class IncorrectPortChar : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class IncorrectPortPick : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class IncorrectPwdSize : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class IncorrectPwdSpace : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

	private:
		void							pingUser(void);
		int								checkPort(char *);
		std::string						checkPwd(char *);

	private:
		int								_fd;
		int								_port;
		std::string						_pwd;
		std::vector<std::string>		_conf;
		std::vector<std::string>		_motd;
		std::vector<std::string>		_quote;
		size_t							_indexquote;
		bool							*_exec;

		std::map<int, User*> 			_users;
		std::map<std::string, Channel*>	_channels;
		//vector with struct pollfd of each user in it:
		std::vector<pollfd>				_fds;
		struct sockaddr_in				_addr;
		std::vector<std::string>		_unavailable_nick;

		std::time_t						_start_time;
		std::string						_server_name;
		std::string						_version;
		std::string						_user_mode;
		std::string						_channel_mode;

		time_t							_timestamp;
};

#endif
