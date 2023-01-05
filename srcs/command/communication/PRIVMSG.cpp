#include "../../../incl/command/command.hpp"

/* Send private messages between users */

/* Parameters: <msgtarget> <text to be sent> */
/* <msgtarget> is usually the nickname of the recipient of the message, or a channel name. */

/* (301)RPL_AWAY */
/* (401)ERR_NOSUCHNICK */
/* (404)ERR_CANNOTSENDTOCHAN */
/* (411)ERR_NORECIPIENT */
/* (412)ERR_NOTEXTTOSEND */
void	PRIVMSG(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	reply;
	std::string	msg_to_send;
	size_t pos = 0;

	if (command.find("TRANSFER") != std::string::npos)
	{
		std::map<std::string, void (*)(std::string &, User *, Server *)> cmd = user->getCommandFunction();
		cmd["TRANSFER"](command, user, server);
		command.erase(0);
		return ;
	}

	if ((pos = command.find(":")) != std::string::npos)
	{
		msg_to_send = command.substr(pos);
		command.erase(pos);
	}

	std::vector<std::string>	command_split = splitCommand(command, " ");
	if (command_split.size() == 1)
	{
		reply.push_back("PRIVMSG");
		user->sendReply(411, user->getPrefix(), reply, NULL);
		reply.clear();
		if (!msg_to_send.size())
			user->sendReply(412, user->getPrefix(), reply, NULL);
		return ;
	}
	else
	{
		if (!msg_to_send.size())
		{
			user->sendReply(412, user->getPrefix(), reply, NULL);
			return ;
		}
		if (isChannelName(command_split.at(1)))
		{
			if (!isChannel(command_split.at(1), server))
			{
				reply.push_back(command_split.at(1));
				user->sendReply(401, user->getPrefix(), reply, NULL);
				reply.clear();
			}
			else if (isOnChannel(user->getFd(), command_split.at(1), server))
				server->getChannel().find(command_split.at(1))->second->sendToAllUser(msg_to_send, "PRIVMSG", user, false);
			else
			{
				reply.push_back(command_split.at(1));
				user->sendReply(404, user->getPrefix(), reply, NULL);
				reply.clear();
			}
		}
		else if (isUser(command_split.at(1), server))
		{
			if (!isUser(command_split.at(1), server))
			{
				reply.push_back(command_split.at(1));
				user->sendReply(401, user->getPrefix(), reply, NULL);
				reply.clear();
			}
			else
			{
				reply.push_back("PRIVMSG");
				reply.push_back(user->getNickname());
				reply.push_back(msg_to_send);
				server->getUser().find(server->getUserFd(command_split.at(1)))->second->sendReply(1005, user->getPrefix(), reply, NULL);
			}
		}
		else
		{
			reply.push_back(command_split.at(1));
			user->sendReply(401, user->getPrefix(), reply, NULL);
			reply.clear();
		}
	}
}

