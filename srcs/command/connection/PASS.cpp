#include "../../../incl/command/command.hpp"

/* Set a connection password */
/* MUST be set before any attempt to register */

/* 	461(ERR_NEEDMOREPARAMS) if not enough parameters given */
/* 	462(ERR_ALREADYREGISTRED) if the user is already registred */

void	PASS(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	command_split = splitCommand(command, " ");
	std::vector<std::string>	reply;

	if (command_split.size() != 2)
	{
		reply.push_back("PASS");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	std::map<int, User*> u = server->getUser();
	std::map<int, User*>::iterator start = u.begin();
	std::map<int, User*>::iterator end = u.end();
	for (; start != end; start++)
	{
		if (user->getUsername() != "__INVALID__")
		{
			user->sendReply(462, user->getPrefix(), reply, NULL);
			return ;
		}
	}

	if (command_split.at(1) == server->getPwd())
		user->setPwd(true);
}
