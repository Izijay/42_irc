#include "../../incl/server/Server.hpp"

Server::Server(char *port, char *pwd, bool *exec): _fd(), _exec(exec), _users(), _channels(), _fds(), _addr()
{
	this->_port = checkPort(port);
	this->_pwd = checkPwd(pwd);
	this->_start_time = std::time(0);
	this->_server_name = "localhost";
	this->_version = "1.0";
	this->_channel_mode = "oik";
	this->_user_mode = "io";
	this->_indexquote = 0;
	
	{
		std::string line;
		std::ifstream myfile("config/ircd.conf");
		if (myfile.is_open())
		{
			while (std::getline(myfile,line))
				this->_conf.push_back(line);
			myfile.close();
		}
	}
	{
		std::srand(time(0));
		std::string line;
		std::ifstream myfile;
		if ((rand() % 10) % 2)
			myfile.open("config/motd.txt");
		else
			myfile.open("config/chirac.txt");
		if (myfile.is_open())
		{
			while (std::getline(myfile,line))
				this->_motd.push_back(line);
			myfile.close();
		}
	}
	{
		std::string line;
		std::ifstream myfile("config/quote.conf");
		if (myfile.is_open())
		{
			while (std::getline(myfile,line))
				this->_quote.push_back(line);
			myfile.close();
		}
	}
	time(&_timestamp);	
}

Server::~Server()
{
	for (std::map<std::string, Channel*>::iterator start = this->_channels.begin(); start != this->_channels.end(); start++)
	{
		delete start->second;
	}
	for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
	{
		close((*start).second->getFd());
		delete start->second;
	}
	close(this->_fd);
	return ;
}

void	Server::error(std::string message, bool exit)
{
	std::cout << "error:" << message << std::endl;
	if (exit)
		std::exit(EXIT_FAILURE);
}

void	Server::init()
{
	int enable_option = 1;
	if ((this->_fd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0)) == -1)
		error("socket", true);
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&enable_option, sizeof(enable_option)) == -1)
		error("setsockopt", true);
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(this->_port);


	if (bind(this->_fd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) == -1)
		error("bind:", true);
	if (listen(this->_fd, SOMAXCONN) == -1)
		error("listen", true);
	this->_fds.push_back(pollfd());
	this->_fds.back().fd = this->_fd; 
	this->_fds.back().events = (POLLIN | POLLRDHUP);
}

void	Server::connectUser(void)
{
	struct sockaddr_in	addr;
	socklen_t	addr_len = sizeof(addr);
	int fd = accept(this->_fd, (sockaddr*)&addr, &addr_len);
	if (fd == -1)
		error("accept", false);
	this->_users[fd] = new User(fd, addr, this);

	this->_fds.push_back(pollfd());
	this->_fds.back().fd = fd;
	this->_fds.back().events = (POLLIN | POLLRDHUP);
}

void	Server::exec()
{
	//Call poll() waiting for fd to be ready
	if (poll(&(this)->_fds.front(), this->_fds.size(), 300) == -1)
		return ;

	//Si le fd == server 
	if (this->_fds.front().revents == POLLIN)
	{
		connectUser();
	}
	//Else, check user's fd for recv()
	else
	{
		for(std::vector<pollfd>::iterator start = this->_fds.begin(); start != this->_fds.end(); )
		{
			if ((*start).revents == POLLIN)
				this->_users[(*start).fd]->receiveMessage();
			if (((*start).revents & POLLRDHUP) == POLLRDHUP)
				delUser(this->_users[(*start).fd], "");
			if ((*start).fd == -1)
				start = this->_fds.erase(start);
			if (start != this->_fds.end())
				start++;
		}
	}
	pingUser();
	autoMessage();
}

void	Server::delUser(User *user, std::string const message)
{
	int fd = user->getFd();
	std::vector<std::string> reply;

	reply.push_back("QUIT");
	reply.push_back(message);
	for (std::map<int, User*>::iterator start_user = this->_users.begin(); start_user != this->_users.end(); start_user++)
	{
		std::vector<Channel*> user_channel = listChannelByUser((*start_user).second->getFd()); 
		for (std::vector<Channel*>::iterator start_user_channel = user_channel.begin(); start_user_channel != user_channel.end(); start_user_channel++)
		{
			std::map<int, User*> user_in_channel = this->_channels[(*start_user_channel)->getName()]->getUsers();
			if (user_in_channel.find(fd) != user_in_channel.end())
			{
				(*start_user).second->sendReply(1005, user->getPrefix(), reply, NULL);
				break ;
			}
		}
	}

	std::vector<Channel*> user_channel = listChannelByUser(fd);
	for (std::vector<Channel*>::iterator start = user_channel.begin(); start != user_channel.end(); start++)
		(*start)->delUser(user);

	std::map<int, User*>::iterator pos;
	if ((pos = this->_users.find(fd)) != this->_users.end())
	{
		delete this->_users[fd];
		this->_users.erase(pos);
	}

	for (std::vector<pollfd>::iterator start = this->_fds.begin(); start != this->_fds.end(); start++)
	{
		if ((*start).fd == fd)
		{
			close(fd);
			(*start).fd = -1;
			return ;
		}
	}
}

void	Server::delChannel(Channel *channel)
{
	if (channel)
	{
		this->_channels.erase(this->_channels.find(channel->getName()));
		delete channel;
	}
}

int	Server::getUserFd(std::string const user_name) const
{
	std::map<int, User*>::const_iterator user_start = this->_users.begin();
	std::map<int, User*>::const_iterator user_end = this->_users.end();
	for ( ; user_start != user_end; user_start++)
	{
		if ((*user_start).second->getNickname() == user_name)
			return ((*user_start).second->getFd());
	}
	return (-1);
}

void	Server::pingUser(void)
{
	std::time_t actual_time = std::time(0);
	std::vector<std::string> reply;
	std::vector<int> timeout_user;

	std::map<int, User*>::const_iterator user_start = this->_users.begin();
	std::map<int, User*>::const_iterator user_end = this->_users.end();
	for ( ; user_start != user_end; user_start++)
	{
		if ((*user_start).second->getLastPing() != 0 && (actual_time - (*user_start).second->getLastPing()) > (USER_TIMEOUT / 2))
		{
			if ((actual_time - (*user_start).second->getFailPing()) > 30)
			{
				(*user_start).second->sendReply(1001, (*user_start).second->getNickname(), reply, NULL);
				(*user_start).second->setFailPing();
			}
		}
		if ((*user_start).second->getLastPing() != 0 && (actual_time - (*user_start).second->getLastPing()) > USER_TIMEOUT)
		{
			(*user_start).second->setTimeout(true);
			timeout_user.push_back((*user_start).second->getFd());
		}
	}

	for (std::vector<int>::iterator user = timeout_user.begin(); user != timeout_user.end(); user++)
		delUser(this->_users.find(*user)->second, "USER_TIMEOUT");
}

std::map<int, User*>	Server::getUser(void) const
{
	return (this->_users);
}

std::map<std::string, Channel*>	Server::getChannel(void) const
{
	return (this->_channels);
}

std::string const	Server::getCreatedDate(void) const
{
	std::tm* now = std::localtime(&this->_start_time);
	std::ostringstream	string_stream;;

	string_stream << (now->tm_mon + 1) \
		<< '-' \
		<< now->tm_mday \
		<< '-' \
		<< (now->tm_year + 1900) \
		<< ' ' \
		<< now->tm_hour \
		<< 'h' \
		<< '-' \
		<< now->tm_min \
		<< 'm' \
		<< '-' \
		<< now->tm_sec \
		<< 's';

	std::string	created_date(string_stream.str());
	return (created_date);
}

std::string const	Server::getPwd(void) const
{
	return (this->_pwd);
}

std::string const	Server::getServerName(void) const
{
	return (this->_server_name);
}

std::string const	Server::getVersion(void) const
{
	return (this->_version);
}

std::string const	Server::getChannelMode(void) const
{
	return (this->_channel_mode);
}

std::string const	Server::getUserMode(void) const
{
	return (this->_user_mode);
}

std::vector<std::string>	Server::getConf(void) const
{
	return (this->_conf);
}

std::vector<std::string>	Server::getMotd(void) const
{
	return (this->_motd);
}

std::vector<std::string>	Server::getUnavaliableNick(void) const
{
	return (this->_unavailable_nick);
}

void	Server::setChannel(std::string const name, std::string const key, User *user)
{
	this->_channels[name] = new Channel(name, key, user, this->_channel_mode);
}

void	Server::setExec(bool status)
{
	*this->_exec = status;
}

std::vector<Channel*>	Server::listChannelByUser(int fd) const
{
	std::vector<Channel*>	channel_list;
	std::map<std::string, Channel*>::const_iterator	start_channel = this->_channels.begin();
	std::map<std::string, Channel*>::const_iterator	end_channel = this->_channels.end();
	for ( ; start_channel != end_channel; start_channel++)
	{
		std::map<int, User*> channel_user = (*start_channel).second->getUsers();
		if (channel_user.find(fd) != channel_user.end())
			channel_list.push_back((*start_channel).second);
	}
	return (channel_list);
}

int	Server::checkPort(char *port)
{
	std::string strport(port);
	for (size_t i = 0; i < strport.size(); i++)
	{
		if (!isdigit(strport[i]))
			throw Server::IncorrectPortChar();
	}
	int digitport = atoi(strport.c_str());
	if (digitport < 1024 || digitport > 49551)
		throw Server::IncorrectPortPick();
	return digitport;
}

std::string	Server::checkPwd(char *password)
{
	std::string pwd(password);
	if (pwd.size() < 6)
		throw Server::IncorrectPwdSize();
	for (size_t i = 0; i < pwd.size(); i++)
	{
		if (pwd[i] == 32)
			throw Server::IncorrectPwdSpace();
	}
	return pwd;
}

void	Server::putUnavailableNick(std::string &nick)
{
	_unavailable_nick.push_back(nick);
}

const char* Server::IncorrectPortChar::what() const throw()
{
	return ("Incorrect port format :\nThe port must contains only digits.");
}

const char* Server::IncorrectPortPick::what() const throw()
{
	return ("Incorrect port format :\nThe port must be between 1024 and 49 551.");
}

const char* Server::IncorrectPwdSize::what() const throw()
{
	return ("Incorrect password format :\nPassword size must be at least 6 characters.");
}

const char* Server::IncorrectPwdSpace::what() const throw()
{
	return ("Incorrect password format :\nNo space allowed in the password.");
}

void	Server::autoMessage()
{
	time_t rep;
	time(&rep);

    unsigned long seconds = difftime(rep, _timestamp);
	if (seconds < TIME_AUTO_MESSAGE)
		return;

	time(&_timestamp);
	time_t secondes;
    struct tm instant;

    time(&secondes);
    instant = *localtime(&secondes);

	std::vector<std::string> reply;
	std::stringstream day;
	day << instant.tm_mday;
	std::stringstream mon;
	mon << instant.tm_mon + 1;
	std::stringstream hour;
	hour << instant.tm_hour;
	std::stringstream min;
	if (instant.tm_min < 10)
		min << 0;
	min << instant.tm_min;
	std::vector<std::string> msg;
	msg.push_back(":---> You currently are on the most beautiful IRC server of the universe. <---\r\n");
	msg.push_back(":* Date of the day : " + day.str() + "/" + mon.str() + " *\r\n");
	msg.push_back(":* Time : " + hour.str() + ":" + min.str() + " *\r\n");
	msg.push_back(":* Proverb : " + _quote.at(_indexquote++) + " *\r\n");
	if (_indexquote == _quote.size())
		_indexquote = 0;
	

	std::map<int, User*>::iterator start = _users.begin();
	std::map<int, User*>::iterator end = _users.end();
	for (; start != end; start++)
	{
		reply.push_back("NOTICE");
		if (start->second->getNickname() != "__INVALID__")
		{
			reply.push_back(start->second->getNickname());
			for (std::vector<std::string>::iterator it = msg.begin(); it != msg.end(); it++)
			{
				reply.push_back((*it));
				start->second->sendReply(1005, ":admin!admin@localhost", reply, NULL);
				reply.pop_back();
			}
		}
		reply.clear();
	}
}

