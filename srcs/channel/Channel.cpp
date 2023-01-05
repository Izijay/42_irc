#include "../../incl/channel/Channel.hpp"

Channel::Channel(std::string const name, std::string const key, User *user, std::string const mode) 
{
	this->_bot = new Bot();
	this->_name = name;
	this->_key = key;
	this->_creator = user;
	this->_operator[user->getFd()] = user;
	this->_invite_only = false;
	this->_mode.push_back('t');
	this->_mode.push_back('o');

	for (std::string::const_iterator start = mode.begin(); start != mode.end(); start++)
		this->_available_mode.push_back(*start);

	acceptUser(user);
	return ;
}

Channel::~Channel()
{
	if (this->_bot)
		delete this->_bot;
}

void	Channel::acceptUser(User *user)
{
	std::vector<std::string>	reply;

	size_t pos;

	if (this->_users.find(user->getFd()) != this->_users.end())
		return ;

	this->_users[user->getFd()] = user;

	if ((pos = user->getMode().find("o")) != std::string::npos \
			&& this->_operator.find(user->getFd()) == this->_operator.end())
		this->_operator[user->getFd()] = user;

	int fd_user = user->getFd();

	for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
	{
		reply.push_back("JOIN");
		reply.push_back(this->_name);
		start->second->sendReply(1002, this->_users[fd_user]->getPrefix(), reply, this);
		reply.clear();
	}

	if (getTopic().size() != 0)
		user->sendReply(332, this->_users[fd_user]->getPrefix(), reply, this);

	for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
	{
		reply.push_back("= " + this->_name);
		if (this->_operator.find((*start).second->getFd()) != this->_operator.end())
			reply.push_back(":@" + start->second->getNickname());
		else
			reply.push_back(":" + start->second->getNickname());
		user->sendReply(353, this->_users[fd_user]->getPrefix(), reply, this);
		reply.clear();
	}

	user->sendReply(366, this->_users[fd_user]->getPrefix(), reply, this);
}

void	Channel::sendListUser(User *user)
{
	std::vector<std::string>	reply;

	for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
	{
		if ((*start).second->getMode().find("i") != std::string::npos)
			continue ;
		reply.push_back("= " + this->_name + " :");
		if (this->_operator.find((*start).first) != this->_operator.end())
			reply.push_back("@" + start->second->getNickname());
		else
			reply.push_back(start->second->getNickname());
		user->sendReply(353, user->getPrefix(), reply, this);
		reply.clear();
	}
	user->sendReply(366, user->getPrefix(), reply, this);
}

void	Channel::sendToAllUser(std::string message, std::string command, User *user, bool to_origin_user)
{
	std::vector<std::string>	reply;
	bool censorship = false;
	size_t pos = 0;
	int fd_user = user->getFd();

	reply.push_back(command);
	reply.push_back(this->_name);


	if ((pos = message.find("!bot")) != std::string::npos && pos == 1)
	{
		if (this->_operator.find(fd_user) == this->_operator.end())
		{
			reply.push_back(":You're not channel operator");
			user->sendReply(1005, this->_bot->getPrefix(), reply, this);
		}
		else if (message != ":!bot on" && message != ":!bot off")
		{
			reply.push_back(":usage: !bot <on / off>");
			user->sendReply(1005, this->_bot->getPrefix(), reply, this);
		}
		else
		{
			this->_bot->setStatus(message);
			if (this->_bot->getStatus() == true)
				message = ":Censorship activated";
			else
				message = ":Censorship deactivated";

			reply.push_back(message);
			for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
				start->second->sendReply(1005, this->_bot->getPrefix(), reply, this);
		}
		return ;
	}

	if (this->_bot->getStatus() == true)
		censorship = this->_bot->censoringMessage(message);
	
	reply.push_back(message);
	for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
	{
		if (!to_origin_user)
		{
			if (start->first != fd_user)
				start->second->sendReply(1005, this->_users[fd_user]->getPrefix(), reply, this);
		}
		else
		{
			start->second->sendReply(1005, this->_users[fd_user]->getPrefix(), reply, this);
		}
	}

	if (censorship)
	{
		reply.pop_back();
		reply.push_back(":Caution! Watch your language!");
		user->sendReply(1005, this->_bot->getPrefix(), reply, this);
	}
}

void	Channel::delUser(User *user)
{
	std::map<int, User*>::iterator user_key = this->_users.find(user->getFd());
	if (user_key != this->_users.end())
		this->_users.erase(user_key);

	std::map<int, User*>::iterator operator_key = this->_operator.find(user->getFd());
	if (operator_key != this->_operator.end())
		this->_operator.erase(operator_key);
}

std::string	const	Channel::getKey(void) const
{
	return (this->_key);
}

std::string	const	Channel::getName(void) const
{
	return (this->_name);
}

std::string const	Channel::getTopic(void) const
{
	return (this->_topic);
}

User *	Channel::getCreator(void) const
{
	return (this->_creator);
}

std::map<int, User*>	Channel::getOperator(void) const
{
	return (this->_operator);
}

std::string const	Channel::getUsersList(void)
{
	std::string user_list;
	for (std::map<int, User*>::iterator start = this->_users.begin(); start != this->_users.end(); start++)
	{
		user_list += start->second->getNickname() + " ";
	}
	return (user_list);
}

std::map<int, User*> const	Channel::getUsers(void) const
{
	return (this->_users);
}

std::vector<char> const	Channel::getMode(void) const
{
	return (this->_mode);
}

std::vector<std::string>	Channel::getInviteList(void) const
{
	return (this->_invite_list);
}

void	Channel::setTopic(std::string const topic_name)
{
	this->_topic = topic_name;
}

void	Channel::setInviteList(std::string const nick)
{
	this->_invite_list.push_back(nick);
}

void	Channel::setMode(bool add, char mode)
{
	if (add)
	{
		if (mode == 'i' && this->_invite_only == false)
		{
			this->_invite_only = true;
			this->_mode.push_back('i');
			return ;
		}
	}
	else
	{
		for (std::vector<char>::iterator start = this->_mode.begin(); start != this->_mode.end(); )
		{
			if (*start == mode)
			{
				if (*start == 'i')
					this->_invite_only = false;
				start = this->_mode.erase(start);
			}
			if (start != this->_mode.end())
				start++;
		}
	}
}

void	Channel::setMode(bool add, char mode, std::string const arg, User *user, Server *server)
{
	std::vector<std::string> reply;
	if (add)
	{
		if (mode == 'o' && arg.size())
		{
			if (this->_operator.find(server->getUserFd(arg)) == this->_operator.end() \
						&& this->_users.find(server->getUserFd(arg)) != this->_users.end())
				this->_operator[server->getUserFd(arg)] = server->getUser().find(server->getUserFd(arg))->second;
			else
			{
				reply.push_back(arg);
				reply.push_back(this->_name);
				user->sendReply(441, user->getPrefix(), reply, NULL);
			}
		}
		if (mode == 'k' && arg.size() && this->_key == "x")
		{
			this->_key = arg;
			this->_mode.push_back('k');
			return ;
		}
	}
	else
	{
		for (std::vector<char>::iterator start = this->_mode.begin(); start != this->_mode.end(); )
		{
			if (*start == mode && *start != 't')
			{
				if (*start == 'k')
				{
					this->_key = "x";
					start = this->_mode.erase(start);
					break ;
				}
				if (*start == 'o')
				{
					if (!isOnChannel(arg, this->_name, server))
					{
						reply.push_back(arg);
						reply.push_back(this->_name);
						user->sendReply(441, user->getPrefix(), reply, NULL);
						if (start != this->_mode.end())
							start++;
						continue;
					}
					User *check_user = server->getUser().find(server->getUserFd(arg))->second;;
					size_t pos;
					if (check_user && (pos = check_user->getMode().find("o")) != std::string::npos)
					{
						if (start != this->_mode.end())
							start++;
						continue;
					}
					else
					{
						this->_operator.erase(this->_operator.find(server->getUserFd(arg)));
						if (start != this->_mode.end())
							start++;
						continue;
					}
				}
				start = this->_mode.erase(start);
			}
			if (start != this->_mode.end())
				start++;
		}
	}
}

void	Channel::checkOper(void)
{
	for (std::map<int, User*>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if ((*it).second->getMode().find("o") != std::string::npos)
		{
			if (this->_operator.find((*it).first) == this->_operator.end())
			{
				this->_operator[(*it).first] = (*it).second;
			}
		}
	}
}
