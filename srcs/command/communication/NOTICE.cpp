#include "../../../incl/command/command.hpp"

/* Parameters: <msgtarget> <text> */

/* The NOTICE command is used similarly to PRIVMSG.  The difference */
/*    between NOTICE and PRIVMSG is that automatic replies MUST NEVER be */
/*    sent in response to a NOTICE message. */

/* See PRIVMSG for more details on replies and examples. */
void	NOTICE(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	reply;
	std::string	msg_to_send;
	size_t pos = 0;

	if ((pos = command.find(":")) != std::string::npos)
	{
		msg_to_send = command.substr(pos);
		command.erase(pos);
	}

	std::vector<std::string>	command_split = splitCommand(command, " ");
	if (command_split.size() == 2)
	{
		reply.push_back("NOTICE");
		reply.push_back(user->getNickname());
		reply.push_back(msg_to_send);
		server->getUser().find(server->getUserFd(command_split.at(1)))->second->sendReply(1005, user->getPrefix(), reply, NULL);
	}
}
