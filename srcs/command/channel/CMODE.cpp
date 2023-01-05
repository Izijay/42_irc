#include "../../../incl/command/command.hpp"

/* Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> ) */

/* Note that there is a maximum limit of three (3) changes per */
/*    command for modes that take a parameter. */

/* (461)ERR_NEEDMOREPARAMS */
/* (467)ERR_KEYSET */
/* (482)ERR_CHANOPRIVSNEEDED */
/* (441)ERR_USERNOTINCHANNEL */
/* (472)ERR_UNKNOWNMODE */
/* (324)RPL_CHANNELMODEIS */
/* (367)RPL_BANLIST */
/* (368)RPL_ENDOFBANLIST */
/* (348)RPL_EXCEPTLIST */
/* (349)RPL_ENDOFEXCEPTLIST */
/* (346)RPL_INVITELIST */
/* (347)RPL_ENDOFINVITELIST */
/* (325)RPL_UNIQOPIS */

/* MODE AVAILABLE: oik */
void	CHANNEL_MODE(std::vector<std::string> &command_split, User *user, Server *server)
{
	std::vector<std::string> reply;
	std::vector<std::string> arg;
	std::string mode;
	bool add;
	
	if (command_split.size() == 2)
	{
		std::vector<char> channel_mode = server->getChannel().find(command_split.at(1))->second->getMode();
		for (std::vector<char>::iterator it_mode = channel_mode.begin(); it_mode != channel_mode.end(); it_mode++)
			mode += *it_mode;
		reply.push_back(mode);
		user->sendReply(324, user->getPrefix(), reply, server->getChannel().find(command_split.at(1))->second);
		return ;
	}

	if (command_split.size() > 4)
	{
		reply.push_back("MODE");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}

	if (command_split.at(2) == "b")
		return ;
	if (!isChannel(command_split.at(1), server) || command_split.at(2).size() > 4)
		return ;

	if (command_split.at(2).at(0) != '+' && command_split.at(2).at(0) != '-')
	{
		reply.push_back("MODE");
		user->sendReply(461, user->getPrefix(), reply, NULL);
		return ;
	}
	else
	{
		if (command_split.at(2).at(0) == '+')
			add = true;
		else
			add = false;
		command_split.at(2).erase(command_split.at(2).begin());
	}

	/* SAVE ARG IN NEW VECTOR */
	for (size_t i = 3; i < command_split.size(); i++)
		arg.push_back(command_split.at(i));

	if (!isChannelMode(command_split.at(2)))
	{
		for (std::string::iterator it_mode = command_split.at(2).begin(); it_mode != command_split.at(2).end(); it_mode++)
		{
			if (!isChannelMode(*it_mode))
			{
				reply.push_back(std::string(1, *it_mode));
				reply.push_back(command_split.at(1));
				user->sendReply(472, user->getPrefix(), reply, NULL);
				reply.clear();
				*it_mode = 'x';
			}
		}
		if (reverseIsChannelMode(command_split.at(2)))
			return ;
	}

	if (!isChanop(user->getFd(), command_split.at(1), server))
	{
		user->sendReply(482, user->getPrefix(), reply, server->getChannel().find(command_split.at(1))->second);
		return ;
	}

	Channel *channel = server->getChannel().find(command_split.at(1))->second;
	for (size_t i = 0; i < command_split.at(2).size(); i++)
	{
		if (command_split.at(2).at(i) == 'i')
			channel->setMode(add, command_split.at(2).at(i));
		else if (command_split.at(2).at(i) == 'k' && add && channel->getKey() != "x")
			user->sendReply(467, user->getPrefix(), reply, NULL);
		else
		{
			if (i < arg.size())
				channel->setMode(add, command_split.at(2).at(i), arg.at(i), user, server);
			else
				channel->setMode(add, command_split.at(2).at(i), "", user, server);
		}
	}

	/* RPL_CHANNELMODEIS AFTER EACH MODE CALL */
	std::vector<char> channel_mode = channel->getMode();
	for (std::vector<char>::iterator it_mode = channel_mode.begin(); it_mode != channel_mode.end(); it_mode++)
		mode += *it_mode;
	reply.push_back(mode);
	user->sendReply(324, user->getPrefix(), reply, server->getChannel().find(command_split.at(1))->second);
}
