#include "../../incl/utils.hpp"

bool	isLetter(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (true);
	return (false);
}

bool	isDigit(char c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

bool	isSpecial(char c)
{
	if ((c >= '[' && c <= '`') || (c >= '{' && c <= '}' ))
		return (true);
	return (false);
}

bool	isOnlySpace(std::string const &str)
{
	for (std::string::const_iterator start = str.begin(); start != str.end(); start++)
	{
		if (*start != ' ')
			return (false);
	}
	return (true);
}

std::vector<std::string>	splitCommand(std::string &command, std::string delimiter)
{
	size_t pos = 0;
	std::vector<std::string>	command_split;

	while ((pos = command.find(delimiter)) != std::string::npos) 
	{
		command_split.push_back(command.substr(0, pos));
		command.erase(0, pos + 1);
	}
	if (command.length())
	{
		command_split.push_back(command.substr(0, command.length()));
		command.erase(0, command.length());
	}
	return (command_split);
}

bool	isModeFlag(std::string const &mode)
{
	size_t i = 1;
	while (i < mode.size() \
			&& (mode.at(i) == 'i' \
			|| mode.at(i) == 'o'))
		i++;

	if (i == mode.size())
		return (true);

	return (false);
}

bool	reverseIsChannelMode(std::string const &mode)
{
	size_t i = 1;
	while (i < mode.size() \
			&& (mode.at(i) != 'o' \
			|| mode.at(i) != 'i' \
			|| mode.at(i) != 't' \
			|| mode.at(i) != 'k' \
			|| mode.at(i) != 'l' \
			|| mode.at(i) != 'b'))
		i++;

	if (i == mode.size())
		return (true);

	return (false);
}

bool	isChannelMode(std::string const &mode)
{
	size_t i = 1;
	while (i < mode.size() \
			&& (mode.at(i) == 'o' \
			|| mode.at(i) == 'v' \
			|| mode.at(i) == 'i' \
			|| mode.at(i) == 'I' \
			|| mode.at(i) == 't' \
			|| mode.at(i) == 'k' \
			|| mode.at(i) == 'l' \
			|| mode.at(i) == 'b' \
			|| mode.at(i) == 'e'))
		i++;

	if (i == mode.size())
		return (true);

	return (false);
}

bool	isChannelMode(char mode)
{
	if (mode == 'o' \
		|| mode == 'i' \
		|| mode == 't' \
		|| mode == 'k')
		return (true);

	return (false);
}

bool	isUserMode(std::string const &user_mode, char mode)
{
	for (std::string::const_iterator start_mode = user_mode.begin(); start_mode != user_mode.end(); start_mode++)
	{
		if (*start_mode == mode)
			return (true);
	}
	return (false);
}

bool	isOnChannel(int user_fd, std::string const &channel_name, Server *server)
{
	if (!isChannel(channel_name, server))
		return (false);
	std::map<int, User*> channel_user = server->getChannel()[channel_name]->getUsers();
	if (channel_user.find(user_fd) != channel_user.end())
		return (true);
	return (false);
}

bool	isOnChannel(std::string user_name, std::string const &channel_name, Server *server)
{
	if (!isChannel(channel_name, server))
		return (false);
	std::map<int, User*> channel_user = server->getChannel()[channel_name]->getUsers();
	std::map<int, User*>::iterator	user_start = channel_user.begin();
	std::map<int, User*>::iterator	user_end = channel_user.end();
	for ( ; user_start != user_end; user_start++)
	{
		if (user_start->second->getNickname() == user_name)
			return (true);
	}
	return (false);
}

bool	isUser(std::string const &name, Server *server)
{
	std::map<int, User*>	user = server->getUser();
	std::map<int, User*>::iterator	user_start = user.begin();
	std::map<int, User*>::iterator	user_end = user.end();
	for ( ; user_start != user_end; user_start++)
	{
		if (user_start->second->getNickname() == name)
			return (true);
	}
	return (false);
}

bool	isChannelName(std::string const &name)
{
	if (name.at(0) == '#' \
			|| name.at(0) == '&' \
			|| name.at(0) == '!' \
			|| name.at(0) == '+')
		return (true);
	return (false);
}

bool	isValidChannelName(std::string const &name)
{
	size_t pos = 0;

	if (name.size() > 50)
		return (false);

	if (!isChannelName(name))
		return (false);

	if ((pos = name.find(" ") != std::string::npos) \
			|| (pos = name.find(",") != std::string::npos) \
			|| (pos = name.find(":") != std::string::npos))
		return (false);

	return (true);
}

bool	isChannel(std::string const &channel_name, Server *server)
{
	std::map<std::string, Channel*> channel = server->getChannel();
	if (channel.find(channel_name) != channel.end())
		return (true);
	return (false);
}

std::string	strcatReply(std::vector<std::string> const reply)
{
	std::string	ret;
	for (std::vector<std::string>::const_iterator start = reply.begin(); start != reply.end(); start++)
	{
		ret += " " + (*start);
	}
	ret += "\r\n";
	return (ret);
}

bool	isChanop(int user_fd, std::string const &channel_name, Server *server)
{
	std::map<int, User*> op_channel = server->getChannel().find(channel_name)->second->getOperator();
	
	if (op_channel.find(user_fd) != op_channel.end())
		return (true);
	return (false);
}

bool	isInviteChannel(std::string const &channel_name, Server *server)
{
	std::vector<char> channel_mode = server->getChannel().find(channel_name)->second->getMode();
	std::vector<char>::const_iterator start_mode = channel_mode.begin();
	std::vector<char>::const_iterator end_mode = channel_mode.end();
	for ( ; start_mode != end_mode; start_mode++)
	{
		if (*start_mode == 'i')
			return (true);
	}
	return (false);
}
