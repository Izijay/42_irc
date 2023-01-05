#include "../../../incl/command/command.hpp"

/* https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.1 */
 /* Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0" */

/* (461)ERR_NEEDMOREPARAMS */
/* (474)ERR_BANNEDFROMCHAN */
/* (473)ERR_INVITEONLYCHAN */
/* (475)ERR_BADCHANNELKEY */
/* (471)ERR_CHANNELISFULL */
/* (403)ERR_NOSUCHCHANNEL to indicate the given channel name is invalid. */
/* (405)ERR_TOOMANYCHANNELS */
/* (407)ERR_TOOMANYTARGETS */
/* (332)RPL_TOPIC => RPL_NOTOPIC || RPL_NAMREPLY */
void	JOIN(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	command_split(splitCommand(command, " "));
	std::vector<std::string>	channel_split;
	std::vector<std::string>	key_split;
	std::map<std::string, std::string> target_map;
	std::vector<std::string>	reply;

	if (command_split.size() < 2)
	{
		reply.push_back("JOIN");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	/* LEAVE ALL CHANNEL */
	/* SEND MESSAGE TO ALL USER IN ALL CHANNEL */
	if (command_split.at(1) == "0")
	{
		std::vector<Channel*> channel_list = server->listChannelByUser(user->getFd());
		for (std::vector<Channel*>::iterator start = channel_list.begin(); start != channel_list.end(); start++)
		{
			reply.push_back("PART");
			reply.push_back((*start)->getName());
			(*start)->sendToAllUser("", "PART", user, false);
			user->sendReply(1011, user->getPrefix(), reply, NULL);
			reply.clear();
			(*start)->delUser(user);
		}
		return  ;
	}

	/* CHECK IF THE CHANNEL NAME IS VALID */
	channel_split = splitCommand(command_split.at(1), ",");
	if (command_split.size() >= 3)
	{
		key_split = splitCommand(command_split.at(2), ",");
		int i = 0;
		for (std::vector<std::string>::iterator start = channel_split.begin(); start != channel_split.end(); start++)
		{
			if (static_cast<size_t>(i) < key_split.size() && key_split.at(i) != "x")
				target_map[*start] = key_split.at(i++);
			else
				target_map[*start] = "x";
		}
	}
	else
	{
		for (std::vector<std::string>::iterator start = channel_split.begin(); start != channel_split.end(); start++)
		{
			target_map[*start] = "x";
		}
	}

	std::map<std::string, std::string>::iterator target_start = target_map.begin();
	std::map<std::string, std::string>::iterator target_end = target_map.end();
	for ( ; target_start != target_end; target_start++)
	{
		std::map<std::string, Channel*> channel = server->getChannel();
		std::map<std::string, Channel*>::iterator target_find = channel.find(target_start->first);
		std::map<std::string, Channel*>::iterator target_end = channel.end();
		if (target_find != target_end)
		{
			if (target_start->second == target_find->second->getKey())
			{
				if (!isInviteChannel(target_find->second->getName(), server))
					target_find->second->acceptUser(user);
				else
				{
					std::vector<std::string> invite_list = target_find->second->getInviteList();
					for (std::vector<std::string>::iterator start = invite_list.begin(); start != invite_list.end(); start++)
					{
						if (*start == user->getNickname())
						{
							target_find->second->acceptUser(user);
							return ;
						}
					}
					user->sendReply(473, user->getPrefix(), reply, target_find->second);
				}
			}
			else
				user->sendReply(475, user->getPrefix(), reply, target_find->second);
		}
		else
		{
			if (!isValidChannelName((*target_start).first))
			{
				reply.push_back((*target_start).first);
				reply.push_back(":No such channel");
				user->sendReply(403, user->getPrefix(), reply, NULL);
				reply.clear();
				continue ;
			}
			server->setChannel(target_start->first, target_start->second, user);
		}
	}
}
