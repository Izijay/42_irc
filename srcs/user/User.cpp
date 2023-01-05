#include "../../incl/user/User.hpp"
#include "../../incl/command/command.hpp"
#include "../../incl/command/replies.hpp"

User::User(int fd, struct sockaddr_in addr, Server *server): 
	_fd(fd),
	_server(server), 
	_last_ping(std::time(0)),
	_fail_ping(std::time(0)),
	_timeout(false),
	_pwd(false),
	_buffer(), 
	_commands_message(), 
	_prefix("__INVALID__"),
	_nickname("__INVALID__"), 
	_username("__INVALID__"), 
	_realname("__INVALID__"),
	_hostname(),
	_mode(), 
	_transfer(false), 
	_put_unavailable(true),
	_try_again(true)
{
	this->_hostaddr = inet_ntoa(addr.sin_addr);
	fcntl(this->_fd, F_SETFL, O_NONBLOCK);

	/* CONNECTION */
	this->_commands_function["PASS"] = &PASS;
	this->_commands_function["NICK"] = &NICK;
	this->_commands_function["USER"] = &USER;
	this->_commands_function["MODE"] = &MODE;
	this->_commands_function["PING"] = &PING;
	this->_commands_function["PONG"] = &PONG;
	this->_commands_function["QUIT"] = &QUIT;
	this->_commands_function["OPER"] = &OPER;
	this->_commands_function["KILL"] = &KILL;

	/* COMMUNICATION */
	this->_commands_function["PRIVMSG"] = &PRIVMSG;
	this->_commands_function["NOTICE"] = &NOTICE;
	this->_commands_function["TRANSFER"] = &TRANSFER;

	/* CHANNEL */
	this->_commands_function["JOIN"] = &JOIN;
	this->_commands_function["PART"] = &PART;
	this->_commands_function["NAMES"] = &NAMES;
	this->_commands_function["TOPIC"] = &TOPIC;
	this->_commands_function["KICK"] = &KICK;
	this->_commands_function["LIST"] = &LIST;
	this->_commands_function["INVITE"] = &INVITE;

	/* QUERIES */
	this->_commands_function["WHO"] = &WHO;
	this->_commands_function["DIE"] = &DIE;
	return ;
}

User::~User() {}

void	User::receiveMessage(void)
{
	char	buffer[BUFFER_SIZE] = "";
	ssize_t	size;
	if ((size = recv(this->_fd, &buffer, BUFFER_SIZE, 0)) == -1)
		return ;

	if (size == static_cast<ssize_t>(BUFFER_SIZE))
	{
		buffer[BUFFER_SIZE - 2] = '\n';
		buffer[BUFFER_SIZE - 1] = '\0';
	}
	this->_buffer += buffer;
	parseMessage();
}

void	User::parseMessage(void)
{
	size_t	pos = 0;
	std::string	buffer = this->_buffer;

	if ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		pos = 0;
		while ((pos = buffer.find("\r\n")) != std::string::npos) 
		{
			this->_commands_message.push_back(buffer.substr(0, pos));
			buffer.erase(0, pos + 2);
		}
		if (this->_commands_message.size() == 1 \
				&& this->_commands_message.at(0) == "CAP LS")
			return ;
	}
	else if ((pos = buffer.find("\n")) != std::string::npos)
	{
		pos = 0;
		while ((pos = buffer.find("\n")) != std::string::npos)
		{
			this->_commands_message.push_back(buffer.substr(0, pos));
			buffer.erase(0, pos + 1);
		}
		
	}
	else
		return ;
	for (std::vector<std::string>::iterator start = this->_commands_message.begin(); start != this->_commands_message.end(); start++)
	{
		if ((*start).find("NICK") != std::string::npos)
		{
			this->_commands_function["NICK"]((*start), this, this->_server);
			if (this->_try_again == false)
			{
				this->_server->delUser(this, "");
				return ;
			}
		}
		if ((*start).find("USER") != std::string::npos)
			this->_commands_function["USER"]((*start), this, this->_server);
		if ((*start).find("MODE") != std::string::npos)
			this->_commands_function["MODE"]((*start), this, this->_server);
		if ((*start).find("PASS") != std::string::npos)
			this->_commands_function["PASS"]((*start), this, this->_server);
		if ((*start).find("PING") != std::string::npos)
			this->_commands_function["PING"]((*start), this, this->_server);
		if ((*start).find("PONG") != std::string::npos)
			this->_commands_function["PONG"]((*start), this, this->_server);
		if ((*start).find("JOIN") != std::string::npos)
			this->_commands_function["JOIN"]((*start), this, this->_server);
		if ((*start).find("TOPIC") != std::string::npos)
			this->_commands_function["TOPIC"]((*start), this, this->_server);
		if ((*start).find("PRIVMSG") != std::string::npos)
			this->_commands_function["PRIVMSG"]((*start), this, this->_server);
		if ((*start).find("NOTICE") != std::string::npos)
			this->_commands_function["NOTICE"]((*start), this, this->_server);
		if ((*start).find("PART") != std::string::npos)
			this->_commands_function["PART"]((*start), this, this->_server);
		if ((*start).find("KICK") != std::string::npos)
			this->_commands_function["KICK"]((*start), this, this->_server);
		if ((*start).find("LIST") != std::string::npos)
			this->_commands_function["LIST"]((*start), this, this->_server);
		if ((*start).find("INVITE") != std::string::npos)
			this->_commands_function["INVITE"]((*start), this, this->_server);
		if ((*start).find("NAMES") != std::string::npos)
			this->_commands_function["NAMES"]((*start), this, this->_server);
		if ((*start).find("WHO") != std::string::npos)
			this->_commands_function["WHO"]((*start), this, this->_server);
		if ((*start).find("OPER") != std::string::npos)
			this->_commands_function["OPER"]((*start), this, this->_server);
		if ((*start).find("kill") != std::string::npos)
			this->_commands_function["KILL"]((*start), this, this->_server);
		if ((*start).find("die") != std::string::npos)
		{
			this->_commands_function["DIE"]((*start), this, this->_server);
			return ;
		}
		if ((*start).find("QUIT") != std::string::npos)
		{
			this->_commands_function["QUIT"]((*start), this, this->_server);
			return ;
		}
	}
	if (this->_pwd == false)
	{
		sendReply(3000, "", std::vector<std::string>(1, ":admin!admin@localhost KILL " + this->_nickname + " :Wrong password, get out!\r\n"), NULL);
		this->_server->delUser(this, "");
		return ;
	}

	if (this->_buffer.size())
		this->_buffer.clear();
	if (this->_commands_message.size())
		this->_commands_message.clear();
}

void	User::sendReply(int num_reply, std::string const prefix, std::vector<std::string> const arg, Channel *channel)
{
	std::string send_buffer;

	switch(num_reply)
	{
		case 001:
			send_buffer = prefix;
			send_buffer += " 001 " +  this->_nickname + " :";
			send_buffer += RPL_WELCOME(this->getNickname(), this->getUsername(), this->getHostname());
			break ;
		case 002:
			send_buffer = prefix;
			send_buffer += " 002 " +  this->_nickname + " :";
			send_buffer += RPL_YOURHOST(this->_server->getServerName(), this->_server->getVersion());
			break ;
		case 003:
			send_buffer = prefix;
			send_buffer += " 003 " +  this->_nickname + " :";
			send_buffer += RPL_CREATED(this->_server->getCreatedDate());
			break ;
		case 004:
			send_buffer = prefix;
			send_buffer += " 004 " +  this->_nickname + " :";
			send_buffer += RPL_MYINFO(this->_server->getServerName(), this->_server->getVersion(), this->_server->getUserMode(), this->_server->getChannelMode());
			break ;
		case 221:
			send_buffer = prefix;
			send_buffer += " 221 " + this->_nickname + " ";
			send_buffer += RPL_UMODEIS(this->_mode);
			break ;
		case 315:
			send_buffer = prefix;
			send_buffer += " 315 " + this->_nickname + " ";
			send_buffer += RPL_ENDOFWHO();
			break ;
		case 322:
			send_buffer = prefix;
			send_buffer += " 322 " + this->_nickname + " ";
			send_buffer += RPL_LIST(channel->getName(), channel->getTopic());
			break ;
		case 323:
			send_buffer = prefix;
			send_buffer += " 323 " + this->_nickname + " ";
			send_buffer += RPL_LISTEND();
			break ;
		case 324:
			send_buffer = prefix;
			send_buffer += " 324 " + this->_nickname + " ";
			send_buffer += RPL_CHANNELMODEIS(arg.at(0), channel);
			break;
		case 331:
			send_buffer = prefix;
			send_buffer += " 331 " + this->_nickname + " ";
			send_buffer += RPL_NOTOPIC(channel); 
			break ;
		case 332:
			send_buffer = prefix;
			send_buffer += " 332 " + this->_nickname + " ";
			send_buffer += RPL_TOPIC(channel); 
			break ;
		case 353:
			send_buffer = prefix;
			send_buffer += " 353 " + this->_nickname;
			send_buffer += strcatReply(arg);
			break ;
		case 341:
			send_buffer = prefix;
			send_buffer += " 341 " + this->_nickname + " ";
			send_buffer += RPL_INVITING(arg.at(0), arg.at(1));
			break ;
		case 366:
			send_buffer = prefix;
			send_buffer += " 366 " + this->_nickname + " ";
			send_buffer += RPL_ENDOFNAMES(channel);
			break ;
		case 372:
			send_buffer = prefix;
			send_buffer += " 372 " + this->_nickname + " ";
			send_buffer += RPL_MOTD(arg.at(0));
			break ;
		case 375:
			send_buffer = prefix;
			send_buffer += " 375 " + this->_nickname + " ";
			send_buffer += RPL_MOTDSTART();
			break ;
		case 376:
			send_buffer = prefix;
			send_buffer += " 376 " + this->_nickname + " ";
			send_buffer += RPL_ENDOFMOTD();
			break ;
		case 381:
			send_buffer = prefix;
			send_buffer += " 381 " + this->_nickname + " ";
			send_buffer += RPL_YOUREOPER();
			break ;
		case 401:
			send_buffer = prefix;
			send_buffer += " 401 " + this->_nickname + " ";
			send_buffer += ERR_NOSUCHNICK(arg.at(0));
			break ;
		case 403:
			send_buffer = prefix;
			send_buffer += " 403 " + this->_nickname;
			send_buffer += ERR_NOSUCHCHANNEL(strcatReply(arg));
			break ;
		case 404:
			send_buffer = prefix;
			send_buffer += " 404 " + this->_nickname + " ";
			send_buffer += ERR_CANNOTSENDTOCHAN(arg.at(0));
			break ;
		case 411:
			send_buffer = prefix;
			send_buffer += " 411 " + this->_nickname + " ";
			send_buffer += ERR_NORECIPIENT(arg.at(0));
			break ;
		case 412:
			send_buffer = prefix;
			send_buffer += " 412 " + this->_nickname + " ";
			send_buffer += ERR_NOTEXTTOSEND();
			break ;
		case 431:
			send_buffer = ERR_NONICKNAMEGIVEN();
			break ;
		case 432:
			send_buffer = ERR_ERRONEUSNICKNAME(arg.at(0));
			break ;
		case 433:
			send_buffer = ERR_NICKNAMEINUSE(arg.at(0));
			break ;
		case 441:
			send_buffer = prefix;
			send_buffer += " 441 " + this->_nickname + " ";
			send_buffer += ERR_USERNOTINCHANNEL(arg.at(0), arg.at(1));
			break ;
		case 442:
			send_buffer = prefix;
			send_buffer += " 442 " + this->_nickname + " ";
			send_buffer += ERR_NOTONCHANNEL(arg.at(0));
			break ;
		case 443:
			send_buffer = prefix;
			send_buffer += " 443 " + this->_nickname + " ";
			send_buffer += ERR_USERONCHANNEL(arg.at(0), arg.at(1));
			break ;
		case 461:
			send_buffer = ERR_NEEDMOREPARAMS(arg.at(0));
			break ;
		case 462:
			send_buffer = ERR_ALREADYREGISTRED();
			break ;
		case 464:
			send_buffer = prefix;
			send_buffer += " 464 " + this->_nickname + " ";
			send_buffer += ERR_PASSWDMISMATCH();
			break ;
		case 472:
			send_buffer = prefix;
			send_buffer += " 472 " + this->_nickname + " ";
			send_buffer += ERR_UNKNOWNMODE(arg.at(0), arg.at(1));
			break ;
		case 473:
			send_buffer = prefix;
			send_buffer += " 473 " + this->_nickname + " ";
			send_buffer += ERR_INVITEONLYCHAN(channel->getName());
			break ;
		case 475:
			send_buffer = prefix;
			send_buffer += " 475 " + this->_nickname + " ";
			send_buffer += ERR_BADCHANNELKEY(channel->getName());
			break ;
		case 481:
			send_buffer = prefix;
			send_buffer += " 481 " + this->_nickname + " ";
			send_buffer += ERR_NOPRIVILEGES();
			break ;
		case 482:
			send_buffer = prefix;
			send_buffer += " 482 " + this->_nickname + " ";
			send_buffer += ERR_CHANOPRIVSNEEDED(channel->getName());
			break ;
		case 501:
			send_buffer = prefix;
			send_buffer += " 501 " + this->_nickname + " ";
			send_buffer += ERR_UMODEUNKNOWNFLAG();
			break ;
		case 502:
			send_buffer = prefix;
			send_buffer += " 502 " + this->_nickname + " ";
			send_buffer += ERR_USERSDONTMATCH();
			break ;
		case 1000:
			send_buffer = "PONG ";
			send_buffer += prefix + "\r\n";
			break ;
		case 1001:
			send_buffer = "PING ";
			send_buffer += prefix + "\r\n";
			break ;
		case 1002:
			send_buffer = prefix;
			send_buffer += strcatReply(arg);
			break ;
		case 1005:
			send_buffer = prefix;
			send_buffer += strcatReply(arg);
			break ;
		case 1011:
			send_buffer = prefix;
			send_buffer += strcatReply(arg);
			break ;
		case 2000:
			send_buffer = prefix;
			send_buffer += strcatReply(arg);
			break ;
		case 3000:
			send_buffer = prefix;
			send_buffer += arg.at(0);
			break ;
	}
	if (send(this->_fd, send_buffer.c_str(), send_buffer.length(), 0) == -1)
		return ;
}


void	User::setNickname(std::string const &nick)
{
	this->_nickname = nick;
}

void	User::setUsername(std::string const &user)
{
	this->_username = user;
}

void	User::setRealname(std::string const &realname)
{
	this->_realname = realname;
}

void	User::setHostaddr(std::string const &hostaddr)
{
	this->_hostaddr = hostaddr;
}

void	User::setHostname(std::string const &hostname)
{
	this->_hostname = hostname;
}

void	User::setPrefix(void)
{
	this->_prefix = ":" + this->_nickname + "!" + this->_username + "@" + this->_hostname;

}

void	User::setMode(std::string const mode, bool oper)
{
	if (mode.at(0) == '+')
	{
		for (std::string::const_iterator start_mode = mode.begin(); start_mode != mode.end(); start_mode++)
		{
			if (*start_mode == 'o')
			{
				if (oper == true)
					this->_mode.push_back(*start_mode);
				continue ;
			}
			if (!isUserMode(this->_mode, *start_mode))
				this->_mode.push_back(*start_mode);
		}
	}
	else
	{
		for (std::string::const_iterator start_mode = mode.begin(); start_mode != mode.end(); start_mode++)
		{
			if (isUserMode(this->_mode, *start_mode))
			{
				for (std::string::iterator mode = this->_mode.begin(); mode != this->_mode.end(); mode++)
				{
					if (*mode == *start_mode)
					{
						this->_mode.erase(mode);
						break ;
					}
				}
			}
		}
	}
	if (this->_mode.size() == 1 && this->_mode.at(0) == '+')
		this->_mode.clear();
}

void	User::setLastPing()
{
	this->_last_ping = std::time(0);
}

void	User::setFailPing()
{
	this->_fail_ping = std::time(0);
}

void	User::setTimeout(bool die)
{
	this->_timeout = die;
}

void	User::setPwd(bool pwd)
{
	this->_pwd = pwd;
}

void	User::setTransfer(bool change)
{
	this->_transfer = change;
}

std::string User::getNickname(void) const
{
	return (this->_nickname);
}

std::string User::getUsername(void) const
{
	return (this->_username);
}

std::string User::getRealname(void) const
{
	return (this->_realname);
}

std::string User::getHostaddr(void) const
{
	return (this->_hostaddr);
}

std::string User::getHostname(void) const
{
	return (this->_hostname);
}

std::vector<std::string>	User::getCommandMessage(void) const
{
	return (this->_commands_message);
}

std::string	User::getMode(void) const
{
	return (this->_mode);
}

int	User::getFd(void) const
{
	return (this->_fd);
}

std::time_t	User::getLastPing(void) const
{
	return (this->_last_ping);
}

std::time_t	User::getFailPing(void) const
{
	return (this->_fail_ping);
}

bool	User::getTimeout(void) const
{
	return (this->_timeout);
}

std::map<std::string, void (*)(std::string &, User *, Server *)>	User::getCommandFunction(void) const
{
	return (this->_commands_function);
}

std::string	User::getPrefix(void) const
{
	return (this->_prefix);
}

bool User::getTransfer(void) const
{
	return (this->_transfer);
}

bool	User::getPutUnavailable(void) const
{
	return (this->_put_unavailable);
}

void	User::setPutUnavailable(bool change)
{
	_put_unavailable = change;
}

bool	User::getTryAgain(void) const
{
	return (this->_try_again);
}

void	User::setTryAgain(bool try_again)
{
	this->_try_again = try_again;
}
