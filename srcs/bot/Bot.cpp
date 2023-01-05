#include "../../incl/bot/Bot.hpp"

Bot::Bot()
{
	this->_status = false;
	this->_censure_word = "&!$%#@";
	this->_prefix = ":IRC-BOT!bot@localhost";

	{
		std::string line;
		std::ifstream myfile("config/bot.conf");
		if (myfile.is_open())
		{
			while (std::getline(myfile,line))
				this->_banned_word.push_back(line);
			myfile.close();
		}
	}
	return ;
}

Bot::~Bot()
{
	return ;
}

std::vector<std::string>	Bot::getBannedWord(void) const
{
	return (this->_banned_word);
}

std::string	Bot::getPrefix(void) const
{
	return (this->_prefix);
}

bool	Bot::getStatus(void) const
{
	return (this->_status);
}

void	Bot::setBannedWord(std::string const word)
{
	this->_banned_word.push_back(word);
}

void	Bot::setStatus(std::string command)
{
	std::vector<std::string> command_split = splitCommand(command, " ");

	if (command_split.size() != 2)
		return ;

	if (command_split.at(1) == "on")
		this->_status = true;
	if (command_split.at(1) == "off")
		this->_status = false;
}

bool	Bot::censoringMessage(std::string &message)
{
	size_t pos = 0;
	bool censorship = false; 

	for (std::vector<std::string>::iterator start = this->_banned_word.begin(); start != this->_banned_word.end(); start++)
	{
		if ((pos = message.find(*start)) != std::string::npos)
		{
			size_t end_word = 0;
			if ((end_word = message.find(" ", pos)) != std::string::npos)
				message.replace(pos, end_word - pos, this->_censure_word);
			else
				message.replace(pos, message.size() - pos, this->_censure_word);
			censorship = true;
		}
	}
	
	return (censorship);
}
