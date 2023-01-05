#include "../../../incl/command/command.hpp"

/* Parameters: <nickname> <channel> */

/* (461)ERR_NEEDMOREPARAMS */
/* (401)ERR_NOSUCHNICK */
/* (442)ERR_NOTONCHANNEL */
/* (443)ERR_USERONCHANNEL */
/* (482)ERR_CHANOPRIVSNEEDED */
/* (341)RPL_INVITING */
/* (301)RPL_AWAY */

void	INVITE(std::string &command, User *user, Server *server)
{
	std::vector<std::string> command_split(splitCommand(command, " "));
	std::vector<std::string> reply;

	if (command_split.size() != 3)
	{
		reply.push_back("INVITE");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}
	
	//CHECK IF INVITE USER EXISTE
	if (!isChannelName(command_split.at(1)) && !isUser(command_split.at(1), server))
	{
		reply.push_back(command_split.at(1));
		user->sendReply(401, user->getPrefix(), reply, NULL);
		return ;
	}

	//CHECK IF USER IS ON CHANNEL
	if (!isOnChannel(user->getFd(), command_split.at(2), server))
	{
		reply.push_back(command_split.at(2));
		user->sendReply(442, user->getPrefix(), reply, NULL);
		return ;
	}
	
	//CHECK IF INVITE USER IS ALREADY ON CHANNEL
	if (isOnChannel(command_split.at(1), command_split.at(2), server))
	{
		reply.push_back(command_split.at(1));
		reply.push_back(command_split.at(2));
		user->sendReply(443, user->getPrefix(), reply, NULL);
		return ;
	}

	//CHECK IF CHANNEL MODE --INITE ONLY?
	if (isInviteChannel(command_split.at(2), server))
	{
		if (!isChanop(user->getFd(), command_split.at(2), server))
		{
			reply.push_back(command_split.at(2));
			user->sendReply(482, user->getPrefix(), reply, NULL);
			return ;
		}
	}

	reply.push_back("INVITE");
	reply.push_back(command_split.at(1));
	reply.push_back(command_split.at(2));
	server->getUser().find(server->getUserFd(command_split.at(1)))->second->sendReply(1005, user->getPrefix(), reply, NULL);
	reply.clear();
	reply.push_back(command_split.at(2));
	reply.push_back(command_split.at(1));
	user->sendReply(341, user->getPrefix(), reply, NULL);
	server->getChannel().find(command_split.at(2))->second->setInviteList(command_split.at(1));
}


