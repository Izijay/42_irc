#include "../../../incl/command/command.hpp"

void	PONG(std::string &command, User *user, Server *server)
{
	(void)command;
	(void)server;

	std::vector<std::string>	reply;
	user->setLastPing();
}
