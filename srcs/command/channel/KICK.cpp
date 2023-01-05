#include "../../../incl/command/command.hpp"

/* Parameters: <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>] */

/* The server MUST NOT send KICK messages with multiple channels or users to clients. */ 

/* (461)ERR_NEEDMOREPARAMS */
/* (403)ERR_NOSUCHCHANNEL */
/* (482)ERR_CHANOPRIVSNEEDED */
/* (441)ERR_USERNOTINCHANNEL */
/* (442)ERR_NOTONCHANNEL */
void	KICK(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	command_split;
	std::vector<std::string>	reply;
	std::string	save_cmd;
	size_t pos = 0;

	if ((pos = command.find(":")) != std::string::npos)
	{
		save_cmd = command.substr(pos);
		command.erase(pos);
	}

	command_split = splitCommand(command, " ");
	if (command_split.size() != 3)
	{
		reply.push_back("KICK");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	std::vector<std::string>	channel_split(splitCommand(command_split.at(1), ","));
	std::vector<std::string>	user_split(splitCommand(command_split.at(2), ","));
	for (std::vector<std::string>::iterator channel_start = channel_split.begin(); channel_start != channel_split.end(); channel_start++)
	{
		if (!isChannelName(*channel_start) || !isChannel(*channel_start, server ))
		{
			reply.push_back(*channel_start);
			reply.push_back(":No such channel");
			user->sendReply(403, user->getPrefix(), reply, NULL);
			reply.clear();
			continue ;
		}
		if (!isOnChannel(user->getFd(), *channel_start, server))
		{
			reply.push_back(*channel_start);
			user->sendReply(442, user->getPrefix(), reply, NULL);
			reply.clear();
			continue ;
		}
		if (!isChanop(user->getFd(), *channel_start, server))
		{
			user->sendReply(482, user->getPrefix(), reply, server->getChannel().find(*channel_start)->second);
			continue ;
		}
		else
		{
			for (std::vector<std::string>::iterator	user_start = user_split.begin(); user_start != user_split.end(); user_start++)
			{
				if (!isOnChannel(*user_start, *channel_start, server))
				{
					reply.push_back(*user_start);
					reply.push_back(*channel_start);
					user->sendReply(441, user->getPrefix(), reply, NULL);
					reply.clear();
					continue ;
				}
				else if (server->getUser().find(server->getUserFd(*user_start))->second->getMode().find("o") != \
						 std::string::npos)
				{
					reply.push_back("PRIVMSG");
					reply.push_back(*channel_start);
					reply.push_back(":Don't play with me! I reign over this domain without sharing!");
					user->sendReply(1005, user->getPrefix(), reply, NULL);

				}
				else
				{
					server->getChannel().find(*channel_start)->second->sendToAllUser((*user_start) + " " + save_cmd, "KICK", user, true); 
					server->getChannel().find(*channel_start)->second->delUser(server->getUser().find(server->getUserFd(*user_start))->second);
				}
			}
		}
	}
}
