#include "../../../incl/command/command.hpp"

/* Parameters: [ <channel> *( "," <channel> ) [ <target> ] ] */
/* the target parameter doesnt match the subjet requirement */

/* If no <channel> parameter is given, a list of all channels and their */
/*    occupants is returned.  At the end of this list, a list of users who */
/*    are visible but either not on any channel or not on a visible channel */
/*    are listed as being on `channel' "*". */

/* (353)RPL_NAMREPLY */
/* (366)RPL_ENDOFNAMES */
void	NAMES(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	command_split(splitCommand(command, " "));

	/* List all channel */
	if (command_split.size() == 1)
	{
		std::map<std::string, Channel*> channels = server->getChannel();
		std::map<std::string, Channel*>::iterator start_channel = channels.begin();
		std::map<std::string, Channel*>::iterator end_channel = channels.end();
		for ( ; start_channel != end_channel; start_channel++)
			(*start_channel).second->sendListUser(user);
		return ;
	}

	/* list of users who are visible but either not on any channel */

	std::vector<std::string>	channel_split = splitCommand(command_split.at(1), ",");
	for (std::vector<std::string>::iterator start = channel_split.begin(); start != channel_split.end(); start++)
	{
		if (isChannelName(*start) && isChannel(*start, server))
			server->getChannel()[*start]->sendListUser(user);
	}
}
