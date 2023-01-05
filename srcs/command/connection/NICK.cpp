#include "../../../incl/command/command.hpp"

/* Set the nickname of the user OR send */ 
/* 	431(ERR_NONICKNAMEGIVEN) if is not found */
/* 	432(ERR_ERRONEUSNICKNAME) if is contains erronus character */
/* 	433(ERR_NICKNAMEINUSE) if is already taken */

/* Defined as follow: ( letter / special ) *8( letter / digit / **special / "-" ) */
/* 	**special = %x5B-60 / %x7B-7D */
/* 				"[", "]", "\", "`", "_", "^", "{", "|", "}" */
void	NICK(std::string &command, User *user, Server *server)
{
	std::vector<std::string>	command_split = splitCommand(command, " ");
	std::vector<std::string>	reply;


	if (command_split.size() != 2)
	{
		user->sendReply(431, user->getPrefix(), reply, NULL);
		return ;
	}

	for (unsigned long int i = 0; i < command_split.at(1).length(); i++)
	{
		if ((isLetter(command_split.at(1).at(i)) \
					|| isDigit(command_split.at(1).at(i)) \
					|| isSpecial(command_split.at(1).at(i))) \
				&& i < 8)
			continue ;
		else
		{
			reply.push_back(command_split.at(1));
			user->sendReply(432, user->getPrefix(), reply, NULL);
			return ;
		}
	}

	std::map<std::string, void (*)(std::string &, User *, Server *)> cmd = user->getCommandFunction();

	std::vector<std::string> n = server->getUnavaliableNick();
	std::vector<std::string>::iterator start_u = n.begin();
	std::vector<std::string>::iterator end_u = n.end();
	for (; start_u != end_u; start_u++)
	{
		if ((*start_u) == command_split.at(1))
		{
			user->sendReply(3000, "", std::vector<std::string>(1, ":admin!admin@localhost KILL " + user->getNickname() + " :Your nickname has been killed!\r\n"), NULL);
			user->setTryAgain(false);
			return ;
		}
	}

	std::map<int, User*> u = server->getUser();
	std::map<int, User*>::iterator start = u.begin();
	std::map<int, User*>::iterator end = u.end();
	for (; start != end; start++)
	{
		if (start->second->getNickname() == command_split.at(1))
		{
			reply.push_back(start->second->getNickname());
			user->sendReply(433, user->getPrefix(), reply, NULL);
			user->sendReply(3000, "", std::vector<std::string>(1, ":admin!admin@localhost KILL " + user->getNickname() + " :Use another nickname!\r\n"), NULL);
			user->setTryAgain(false);
			return ;
		}
	}

	if (user->getNickname() != "__INVALID__")
	{
		reply.push_back("NICK");
		reply.push_back(command_split.at(1));
		user->sendReply(1005, user->getPrefix(), reply, NULL);
		user->setNickname(command_split.at(1));
		reply.clear();
	}
	else
	{
		user->setNickname(command_split.at(1));
	}

	if (user->getUsername() != "__INVALID__")
		user->setPrefix();
}
