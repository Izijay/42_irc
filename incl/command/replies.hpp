#ifndef REPLIES_HPP
#define REPLIES_HPP

#define END_OF_MESSAGE "\r\n"

#include <string>
#include "../channel/Channel.hpp"


std::string	RPL_WELCOME(std::string const &nick, std::string const &user, std::string const &hostname); //001
std::string	RPL_YOURHOST(std::string const &servername, std::string const &version); //002
std::string	RPL_CREATED(std::string const &date); //003
std::string	RPL_MYINFO(std::string const &servername, std::string const &version, std::string const &user_mode, std::string const &channel_mode); //004

/* 200 - 399 */
std::string	RPL_UMODEIS(std::string const &user_mode); //221
std::string	RPL_AWAY(std::string const &nick); //301
std::string	RPL_ENDOFWHO(void); //315
std::string	RPL_LIST(std::string const &channel_name, std::string const &channel_topic); //322
std::string	RPL_LISTEND(void); //323
std::string	RPL_CHANNELMODEIS(std::string const &channel_mode, Channel *channel); //324
std::string	RPL_NOTOPIC(Channel *channel); //331
std::string	RPL_TOPIC(Channel *channel); //332
std::string	RPL_INVITING(std::string const &channel_name, std::string const &nick); //341
std::string	RPL_NAMREPLY(Channel *channel); //353
std::string	RPL_ENDOFNAMES(Channel *channel); //366
std::string	RPL_YOUREOPER(void); //381

/* 400 - 599 */
std::string	ERR_NOSUCHNICK(std::string const &nick_or_channel_name); //401
std::string	ERR_NOSUCHCHANNEL(std::string const &channel_name); //403
std::string	ERR_CANNOTSENDTOCHAN(std::string const &channel_name); //404
std::string	ERR_TOOMANYCHANNELS(std::string const &channel_name); //405
std::string	ERR_NORECIPIENT(std::string const &command); //411
std::string	ERR_NOTEXTTOSEND(void); //412
std::string	ERR_NONICKNAMEGIVEN(void); //431
std::string	ERR_ERRONEUSNICKNAME(std::string const &nick); //432
std::string	ERR_NICKNAMEINUSE(std::string const &nick); //433
std::string	ERR_UNAVAILRESOURCE(std::string const &channel_name, std::string const &nick); //437
std::string	ERR_USERNOTINCHANNEL(std::string const &nick, std::string const &channel_name); //441
std::string	ERR_NOTONCHANNEL(std::string const &channel_name); //442
std::string	ERR_USERONCHANNEL(std::string const &nick, std::string const &channel_name); //443
std::string	ERR_NEEDMOREPARAMS(std::string const &cmd); //461
std::string	ERR_ALREADYREGISTRED(void); //462
std::string	ERR_PASSWDMISMATCH(void); //464
std::string	ERR_UNKNOWNMODE(std::string const &mode, std::string const &channel_name); //472
std::string	ERR_INVITEONLYCHAN(std::string const &channel_name); //473
std::string	ERR_BANNEDFROMCHAN(std::string const &channel_name); //474
std::string	ERR_BADCHANNELKEY(std::string const &channel_name); //475
std::string	ERR_BADCHANMASK(std::string const &channel_name); //476
std::string	ERR_NOCHANMODES(std::string const &channel_name); //477
std::string	ERR_NOPRIVILEGES(void); // 481
std::string	ERR_CHANOPRIVSNEEDED(std::string const &channel_name); //482
std::string	ERR_CANTKILLSERVER(std::string const &server); // 483
std::string	ERR_UMODEUNKNOWNFLAG(void); //501
std::string	ERR_USERSDONTMATCH(void); //502

/* 1000 - XXXX CUSTOM_REPLIES */
std::string	RPL_CHANNEL_CMD(std::string const &channel_cmd_list); //1002

/* MOTD */
std::string	RPL_MOTDSTART(void); //375
std::string	RPL_MOTD(std::string const &line); //372
std::string	RPL_ENDOFMOTD(void); //376

#endif
