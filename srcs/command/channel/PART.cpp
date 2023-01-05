#include "../../../incl/command/command.hpp"

/* Parameters: <channel> *( "," <channel> ) [ <Part Message> ] */

/* (461)ERR_NEEDMOREPARAMS */
/* (403)ERR_NOSUCHCHANNEL */
/* (442)ERR_NOTONCHANNEL */
void	PART(std::string &command, User *user, Server *server)
{
	std::vector<std::string> cmd_channel;
	std::vector<std::string>	reply;
	std::string	cmd_msg;
	std::string	cmd_tmp;
	size_t	pos = 0;
	int	i = 0;

	if (command.length() < 6)
	{
		reply.push_back("PART");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	while (i++ != 1 && (pos = command.find(" ")) != std::string::npos)
		command.erase(0, pos + 1);

	pos = command.find(" ");
	if (pos != std::string::npos)
	{
		cmd_tmp = command.substr(0, pos);
		command.erase(0, pos + 1);
		cmd_channel = splitCommand(cmd_tmp, ",");
	}
	else
	{
		cmd_channel = splitCommand(command, ",");
	}

	pos = command.find(":");
	if (pos != std::string::npos)
	{
		cmd_msg = command.substr(pos);
		command.erase(0);
	}

	/* NEED TO CHECK IF USER IN ON CHANNEL (442)*/
	if (!cmd_msg.size())
	{
		for (std::vector<std::string>::iterator start = cmd_channel.begin(); start != cmd_channel.end(); start++)
		{
			if (isChannel(*start, server))
			{
				std::map<int, User*> channel_users = server->getChannel()[*start]->getUsers();
				if (channel_users.find(user->getFd()) != channel_users.end())
				{
					reply.push_back("PART");
					reply.push_back(*start);
					server->getChannel()[*start]->sendToAllUser("", "PART", user, false);
					user->sendReply(1011, user->getPrefix(), reply, NULL);
					server->getChannel()[*start]->delUser(user);
					if (server->getChannel()[*start]->getUsers().empty())
						server->delChannel(server->getChannel()[*start]);
					reply.clear();
				}
				else
				{
					reply.push_back(*start);
					user->sendReply(442, user->getPrefix(), reply, NULL);
					reply.clear();
				}
			}
			else
			{
				reply.push_back(*start);
				reply.push_back(":No such channel");
				user->sendReply(403, user->getPrefix(), reply, NULL);
				reply.clear();
			}
		}
	}

	else
	{
		for (std::vector<std::string>::iterator start = cmd_channel.begin(); start != cmd_channel.end(); start++)
		{
			if (isChannel(*start, server))
			{
				std::map<int, User*> channel_users = server->getChannel()[*start]->getUsers();
				if (channel_users.find(user->getFd()) != channel_users.end())
				{
					reply.push_back("PART");
					reply.push_back(*start);
					reply.push_back(cmd_msg);
					server->getChannel()[*start]->sendToAllUser(cmd_msg, "PART", user, false);
					user->sendReply(1011, user->getPrefix(), reply, NULL);
					server->getChannel()[*start]->delUser(user);
					if (server->getChannel()[*start]->getUsers().empty())
						server->delChannel(server->getChannel()[*start]);
					reply.clear();
				}
				else
				{
					reply.push_back(*start);
					user->sendReply(442, user->getPrefix(), reply, NULL);
					reply.clear();
				}
			}
			else
			{
				reply.push_back(*start);
				reply.push_back(":No such channel");
				user->sendReply(403, user->getPrefix(), reply, NULL);
				reply.clear();
			}
		}
	}
}
