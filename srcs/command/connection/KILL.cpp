#include "../../../incl/command/command.hpp"

// ERR_NOPRIVILEGES // 481
// ERR_NEEDMOREPARAMS // 461
// ERR_NOSUCHNICK // 401
// ERR_CANTKILLSERVER // 483

// Parameters: <nickname> <comment>

void	KILL(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	reply;
	std::string					msg_to_send;
	size_t pos = 0;

	if ((pos = command.find(":")) != std::string::npos)
	{
		msg_to_send = command.substr(pos);
		command.erase(pos);
	}
	std::vector<std::string>	command_split = splitCommand(command, " ");

	if (command_split.size() == 2 && msg_to_send.size() > 1)
	{
		if (user->getMode().find("o") != std::string::npos)
		{
			std::map<int, User*> server_user = server->getUser();
			if (server_user.find(server->getUserFd(command_split.at(1))) != server_user.end())
			{
				if (user->getPutUnavailable() == true)
					server->putUnavailableNick(command_split.at(1));
				user->setPutUnavailable(true);
				server->getUser().find(server->getUserFd(command_split.at(1)))->second->sendReply(3000, user->getPrefix(), std::vector<std::string>(1, " KILL " + command_split.at(1) + " " + msg_to_send + "\r\n"), NULL);
				server->delUser(server->getUser().find(server->getUserFd(command_split.at(1)))->second, "");
				return ;
			}
			else
			{
				reply.push_back(command_split.at(1));
				user->sendReply(401, user->getPrefix(), reply, NULL); // ERR_NOSUCHNICK
			}
		}
		else
		{
			reply.push_back(user->getNickname());
			user->sendReply(481, user->getPrefix(), reply, NULL); // ERR_NOPRIVILEGES
		}
	}
	else
	{
		reply.push_back(command_split.at(0));
		user->sendReply(461, user->getPrefix(), reply, NULL); // ERR_NEEDMOREPARAMS
	}
}