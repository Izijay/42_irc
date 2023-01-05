#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include "user/User.hpp"
#include "server/Server.hpp"

bool	isLetter(char);
bool	isDigit(char);
bool	isSpecial(char);
bool	isOnlySpace(std::string const &str);
bool	isUser(std::string const &, Server*);
bool	isChannelName(std::string const &);
bool	isValidChannelName(std::string const &);
bool	isChannel(std::string const &, Server*);
bool	isOnChannel(int, std::string const &, Server *);
bool	isOnChannel(std::string, std::string const &, Server *);
bool	isChanop(int, std::string const &, Server *);
bool	isModeFlag(std::string const &);
bool	isUserMode(std::string const &, char);
bool	isChannelMode(char);
bool	isChannelMode(std::string const &);
bool	reverseIsChannelMode(std::string const &);
bool	isInviteChannel(std::string const &, Server *);

std::vector<std::string>	splitCommand(std::string &command, std::string delimiter);
std::string	strcatReply(std::vector<std::string> const reply);

#endif
