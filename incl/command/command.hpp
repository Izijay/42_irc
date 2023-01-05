#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../utils.hpp"

/* CONNECTION */
void	PASS(std::string &, User *, Server *);
void	NICK(std::string &, User *, Server *);
void	USER(std::string &, User *, Server *);
void	MODE(std::string &, User *, Server *);
void	PONG(std::string &, User *, Server *);
void	PING(std::string &, User *, Server *);
void	QUIT(std::string &, User *, Server *);
void	KILL(std::string &, User *, Server *);

/* CHANNEL */
void	JOIN(std::string &, User *, Server *);
void	PART(std::string &, User *, Server *);
void	CHANNEL_MODE(std::vector<std::string> &, User *, Server *);
void	NAMES(std::string &, User *, Server *);
void	TOPIC(std::string &, User *, Server *);
void	KICK(std::string &, User *, Server *);
void	LIST(std::string &, User *, Server *);
void	INVITE(std::string &, User *, Server *);
void	OPER(std::string &, User *, Server *);

/* COMMUNICATION */
void	PRIVMSG(std::string &, User *, Server *);
void	NOTICE(std::string &, User *, Server *);
void	TRANSFER(std::string &, User *, Server *);

/* QUERIES */
void	WHO(std::string &, User *, Server *);
void	DIE(std::string &, User *, Server *);
#endif
