#include "../../../incl/command/command.hpp"

/* Parameters: None */
/* (481)ERR_NOPRIVILEGES */

void	DIE(std::string &command, User *user, Server *server)
{
	(void)command;
	size_t pos;
	std::vector<std::string> reply;

	if ((pos = user->getMode().find("o")) != std::string::npos)
		server->setExec(false);
	else
		user->sendReply(481, user->getPrefix(), reply, NULL);
}
