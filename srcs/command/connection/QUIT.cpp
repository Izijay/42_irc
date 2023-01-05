#include "../../../incl/command/command.hpp"

/* Parameters: [ <Quit Message> ] */
/* Numeric Replies: None. */

void	QUIT(std::string &command, User *user, Server *server)
{
	size_t pos = 0;
	std::vector<std::string> reply;

	pos = command.find(":");
	command.erase(0, pos);

	reply.push_back("QUIT");
	reply.push_back(command);

	if (user->getUsername() != "__INVALID__")
	{
		user->sendReply(2000, user->getPrefix(), reply, NULL);
	}
	server->delUser(user, command);
}
