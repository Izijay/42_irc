#include "../../../incl/command/command.hpp"

/* Parameters: <nickname> *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) ) */

/* A user MODE command MUST only be accepted if both the sender of the */
/* message and the nickname given as a parameter are both the same.  If */
/* no other parameter is given, then the server will return the current */
/* settings for the nick. */

/* The available modes are as follows: */
/*        a - user is flagged as away; */
/*        i - marks a users as invisible; */
/*        w - user receives wallops; */
/*        r - restricted user connection; */
/*        o - operator flag; */

/* (461)ERR_NEEDMOREPARAMS if not enough parameters given */ 
/* (502)ERR_USERSDONTMATCH if the user trying to view or change mode for another user */
/* (501)ERR_UMODEUNKNOWNFLAG if the mode flag sent was not recognized. */
/* (221)RPL_UMODEIS if the user want to view his own mode */
void	MODE(std::string &command, User *user, Server *server)
{
	(void)server;
	std::vector<std::string>	command_split = splitCommand(command, " ");
	std::vector<std::string>	reply;

	if (command_split.size() == 1)
	{
		reply.push_back("MODE");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}
	if (isChannelName(command_split.at(1)))
	{
		CHANNEL_MODE(command_split, user, server);
		return ;
	}
	else
	{

		if (command_split.size() == 2 && command_split.at(1) == user->getNickname())
		{
			user->sendReply(221, user->getPrefix(), reply, NULL);
			return ;
		}

		if (command_split.at(1) != user->getNickname())
		{
			user->sendReply(502, user->getPrefix(), reply, NULL);
			return ;
		}

		if (command_split.at(2).at(0) != '+' && command_split.at(2).at(0) != '-')
		{
			reply.push_back("MODE");
			user->sendReply(461, user->getPrefix(), reply, NULL);
			return ;
		}

		if (!isModeFlag(command_split.at(2)) || command_split.size() > 3)
		{
			user->sendReply(501, user->getPrefix(), reply, NULL);
			return ;
		}

		user->setMode(command_split.at(2), false);
		user->sendReply(221, user->getPrefix(), reply, NULL);
	}
}
