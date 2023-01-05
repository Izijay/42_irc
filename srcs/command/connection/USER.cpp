#include "../../../incl/command/command.hpp"

/* Save new user OR send */
/* 	461(ERR_NEEDMOREPARAMS) if not enough parameters given */
/* 	462(ERR_ALREADYREGISTRED) if the user is already registred */

/* Parameters: <user> <mode> <unused> <realname> */

void	USER(std::string &command, User *user, Server *server)
{
	size_t pos = 0;
	std::string username;
	std::string realname;
	std::string hostname;
	std::vector<std::string> command_split;
	std::vector<std::string>	reply;

	reply.push_back("USER");
	if ((pos = command.find(":")) && pos == std::string::npos)
	{
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}
	else
	{
		realname = command.substr(pos + 1, command.length());
		command.erase(pos - 1, command.length());
	}

	command_split = splitCommand(command, " ");
	if (command_split.size() != 4)
	{
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}
	else
	{
		username = command_split.at(1);
		hostname = command_split.at(3);
	}

	if (!username.size() || !realname.size() || isOnlySpace(realname) || !hostname.size())
	{
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	std::map<int, User*> u = server->getUser();
	std::map<int, User*>::iterator start = u.begin();
	std::map<int, User*>::iterator end = u.end();
	for (; start != end; start++)
	{
		if (start->second->getUsername() == username && user->getFd() != start->first)
		{
			user->sendReply(462, user->getPrefix(), reply, NULL);
			return ;
		}
	}

	if (user->getNickname() == "__INVALID__")
	{
		reply.clear();
		reply.push_back("NOTICE");
		reply.push_back(user->getNickname());
		reply.push_back(":No nickname set!");
		user->sendReply(1005, ":admin!admin@localhost", reply, NULL);
		return ;
	}

	user->setUsername(username);
	user->setRealname(realname);
	user->setHostname(hostname);
	user->setPrefix();
	user->sendReply(001, user->getPrefix(), reply, NULL);
	user->sendReply(002, user->getPrefix(), reply, NULL);
	user->sendReply(003, user->getPrefix(), reply, NULL);
	user->sendReply(004, user->getPrefix(), reply, NULL);
	user->sendReply(375, user->getPrefix(), reply, NULL);
	std::vector<std::string> art = server->getMotd();
	for (std::vector<std::string>::iterator start = art.begin(); start != art.end(); start++)
	{
		reply.clear();
		reply.push_back((*start));
		user->sendReply(372, user->getPrefix(), reply, NULL);
	}
	user->sendReply(376, user->getPrefix(), reply, NULL);
}
