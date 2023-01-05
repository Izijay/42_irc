#include "../../../incl/command/command.hpp"

void	PING(std::string &command, User *user, Server *server)
{
	(void)command;
	(void)server;

	std::vector<std::string>	reply;
	user->sendReply(1000, user->getPrefix(), reply, NULL);
	user->setLastPing();
}
