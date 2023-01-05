#include "../../../incl/command/command.hpp"

/* Parameters: <name> <password> */

/* (461)ERR_NEEDMOREPARAMS */
/* (381)RPL_YOUREOPER */
/* (491)ERR_NOOPERHOST */
/* (464)ERR_PASSWDMISMATCH */

void	OPER(std::string &command, User *user, Server *server)
{
	std::vector<std::string> command_split = splitCommand(command, " ");
	std::vector<std::string> reply;
	std::vector<std::string> conf;

	if (command_split.size() != 3)
	{
		reply.push_back("OPER");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	conf = server->getConf();
	for (std::vector<std::string>::iterator start = conf.begin(); start != conf.end(); start++)
	{
		size_t pos = 0;
		if ((pos = (*start).find("=")) != std::string::npos)
			(*start).erase(0, pos +1);
	}
	if (command_split.at(1) != conf.at(0))
	{
		return ;
	}
	if (command_split.at(2) != conf.at(1))
	{
		user->sendReply(464, user->getPrefix(), reply, NULL);
		return ;
	}

	user->setMode("+o", true);
	user->sendReply(381, user->getPrefix(), reply, NULL);
	user->sendReply(221, user->getPrefix(), reply, NULL);

	std::vector<Channel*> user_channel = server->listChannelByUser(user->getFd());
	for (std::vector<Channel*>::iterator it = user_channel.begin(); it != user_channel.end(); it++)
		(*it)->checkOper();
}
