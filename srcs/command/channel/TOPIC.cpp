#include "../../../incl/command/command.hpp"

/* Parameters: <channel> [ <topic> ] */

/* (461)ERR_NEEDMOREPARAMS */
/* (442)ERR_NOTONCHANNEL */
/* (331)RPL_NOTOPIC */
/* (332)RPL_TOPIC */
/* (482)ERR_CHANOPRIVSNEEDED */
/* (477)ERR_NOCHANMODES */
void	TOPIC(std::string &command, User *user, Server *server)
{
	std::string	save_cmd;
	std::vector<std::string>	reply;
	std::vector<std::string>	command_split;
	size_t pos = 0;

	if ((pos = command.find(":")) != std::string::npos)
	{
		save_cmd = command.substr(pos + 1);
		if (isOnlySpace(save_cmd))
		{
			save_cmd.clear();
			save_cmd.push_back(':');
		}
	}

	command_split = splitCommand(command, " ");
	if (command_split.size() == 1)
	{
		reply.push_back("TOPIC");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}


	if (isChannelName(command_split.at(1)) && isChannel(command_split.at(1), server))
	{
		if (!isOnChannel(user->getFd(), command_split.at(1), server))
		{
			reply.push_back(command_split.at(1));
			user->sendReply(442, user->getPrefix(), reply, NULL);
			return ;
		}
		if (!save_cmd.size() && server->getChannel()[command_split.at(1)]->getTopic().size() == 0)
			user->sendReply(331, user->getPrefix(), reply, server->getChannel()[command_split.at(1)]);
		else if (!save_cmd.size() && server->getChannel()[command_split.at(1)]->getTopic().size() != 0)
		{
			user->sendReply(332, user->getPrefix(), reply, server->getChannel()[command_split.at(1)]);
		}
		else
		{
			if (!isChanop(user->getFd(), command_split.at(1), server))
			{
				user->sendReply(482, user->getPrefix(), reply, server->getChannel()[command_split.at(1)]);
			}
			else
			{
				if (save_cmd.size() == 1 && save_cmd.at(0) == ':')
					save_cmd = "";
				server->getChannel()[command_split.at(1)]->setTopic(save_cmd);
				server->getChannel()[command_split.at(1)]->sendToAllUser(server->getChannel()[command_split.at(1)]->getTopic(), "TOPIC", user, 1);
			}
		}
	}

}
