#include "../../../incl/command/command.hpp"

std::string getSuggestedPort()
{
	std::stringstream ss;
	srand((unsigned int)time(0));
	int port = 49551 + rand() % (65535 + 1 - 49551);
	ss << port;
	std::string rep = ss.str();
	return rep;
}

void	TRANSFER(std::string &command, User *user, Server *server)
{
	std::string cmd(command);
	std::vector<std::string>	command_split = splitCommand(command, " ");
	std::vector<std::string> reply;
	std::string msg_to_send;

	if (cmd.find("SEND") != std::string::npos && cmd.find("GET") == std::string::npos && command_split.size() == 5)
	{
		if (user->getTransfer() == false)
		{
			msg_to_send = ":File transfer request has been sent to " + command_split.at(1);
			reply.push_back("PRIVMSG");
			reply.push_back(command_split.at(1));
			reply.push_back(msg_to_send);
			user->sendReply(1005, user->getPrefix(), reply, NULL);
			reply.clear();

			msg_to_send = ":File transfer request receive from " + user->getNickname() + ", type TRANSFER GET to get the file [" + command_split.at(4) + "] or ignore it";
			reply.push_back("PRIVMSG");
			reply.push_back(command_split.at(1));
			reply.push_back(msg_to_send);
			server->getUser().find(server->getUserFd(command_split.at(1)))->second->sendReply(1005, user->getPrefix(), reply, NULL);
			reply.clear();
			user->setTransfer(true);
		}
		else
		{
			msg_to_send = ":Request already in progress";
			reply.push_back("PRIVMSG");
			reply.push_back(command_split.at(1));
			reply.push_back(msg_to_send);
			user->sendReply(1005, user->getPrefix(), reply, NULL);
			reply.clear();
		}
		return ;
	}
	if (cmd.find("GET") != std::string::npos && cmd.find("SEND") == std::string::npos && command_split.size() == 4)
	{
		if (server->getUser().find(server->getUserFd(command_split.at(1)))->second->getTransfer() == true)
		{
			std::string suggested_port = getSuggestedPort();
			msg_to_send = ":File transfer request accepted --> type on terminal : nc -vl " + suggested_port + " > [file to receive]";
			reply.push_back("PRIVMSG");
			reply.push_back(command_split.at(1));
			reply.push_back(msg_to_send);
			user->sendReply(1005, user->getPrefix(), reply, NULL);
			reply.clear();

			msg_to_send = ":File transfer request accepted --> type on terminal : nc -N -w 3 " + server->getUser().find(server->getUserFd(user->getNickname()))->second->getHostaddr() + " " + suggested_port + " < [file to send]";
			reply.push_back("PRIVMSG");
			reply.push_back(command_split.at(1));
			reply.push_back(msg_to_send);
			server->getUser().find(server->getUserFd(command_split.at(1)))->second->sendReply(1005, user->getPrefix(), reply, NULL);
			reply.clear();
			server->getUser().find(server->getUserFd(command_split.at(1)))->second->setTransfer(false);
		}
		else
		{
			msg_to_send = ":No request detected";
			reply.push_back("PRIVMSG");
			reply.push_back(command_split.at(1));
			reply.push_back(msg_to_send);
			user->sendReply(1005, user->getPrefix(), reply, NULL);
			reply.clear();
		}
		return ;
	}
	msg_to_send = ":Usage : TRANSFER SEND [file] to send the file / TRANSFER GET to receive the file";
	reply.push_back("PRIVMSG");
	reply.push_back(command_split.at(1));
	reply.push_back(msg_to_send);
	user->sendReply(1005, user->getPrefix(), reply, NULL);
	reply.clear();
}