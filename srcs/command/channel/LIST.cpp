#include "../../../incl/command/command.hpp"

/* Parameters: [ <channel> *( "," <channel> ) [ <target> ] ] */
/* the target parameter doesnt match the subjet requirement */

 /* The list command is used to list channels and their topics.  If the */
 /*   <channel> parameter is used, only the status of that channel is */
 /*   displayed. */

/* (322)RPL_LIST */
/* (323)RPL_LISTEND */

void	LIST(std::string &command, User *user, Server *server)
{
	std::vector<std::string> reply;
	std::vector<std::string> command_split;

	command.erase(0, 5);
	command_split = splitCommand(command, ",");
	std::map<std::string, Channel*>	channel = server->getChannel();
	if (!command_split.size())
	{
		for (std::map<std::string, Channel*>::iterator start = channel.begin(); start != channel.end(); start++)
				user->sendReply(322, user->getPrefix(), reply, (*start).second);
	}
	else
	{
		for (std::vector<std::string>::iterator start = command_split.begin(); start != command_split.end(); start++)
		{
			if (channel.find(*start) != channel.end())
				user->sendReply(322, user->getPrefix(), reply, server->getChannel().find(*start)->second);
		}
	}
	user->sendReply(323, user->getPrefix(), reply, NULL);
}

