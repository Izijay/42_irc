#include "../../../incl/command/command.hpp"

void	WHO(std::string &command, User *user, Server *server)
{
	std::vector<std::string> reply;
	(void)command;
	(void)server;

	user->sendReply(315, user->getPrefix(), reply, NULL);
}
