#include "../../../incl/command/replies.hpp"

/* 001 - 099 */
std::string	RPL_WELCOME(std::string const &nick, std::string const &user, std::string const &hostname) { return ("Welcome to the Internet Relay Network " + nick+ "!" + user + "@" + hostname + END_OF_MESSAGE); } //001
std::string	RPL_YOURHOST(std::string const &servername, std::string const &version) { return ("Your host is " + servername + ", running version " + version + END_OF_MESSAGE); } //002
std::string	RPL_CREATED(std::string const &date) { return ("This server was created " + date + END_OF_MESSAGE); } //003
std::string	RPL_MYINFO(std::string const &servername, std::string const &version, std::string const &user_mode, std::string const &channel_mode) { return (servername + " " + version + " " + user_mode + " " + channel_mode + END_OF_MESSAGE); } //004

/* 200 - 399 */
std::string	RPL_UMODEIS(std::string const &user_mode) { return (":" + user_mode + END_OF_MESSAGE); } //221
std::string	RPL_AWAY(std::string const &nick) { return (nick + " :<away message>" + END_OF_MESSAGE); } //301
std::string	RPL_ENDOFWHO(void) { return (":End of WHO list\r\n"); } //315
std::string	RPL_LIST(std::string const &channel_name, std::string const &channel_topic) { return (channel_name + " :" + channel_topic + END_OF_MESSAGE); } //322
std::string	RPL_LISTEND(void) { return (":End of LIST\r\n"); } //323
std::string	RPL_CHANNELMODEIS(std::string const &channel_mode, Channel *channel) { return (channel->getName() + " " + channel_mode + END_OF_MESSAGE); } //324
std::string	RPL_NOTOPIC(Channel *channel) { return (channel->getName() + " :No topic is set" + END_OF_MESSAGE); } //331
std::string	RPL_TOPIC(Channel *channel) { return (channel->getName() + " :" + channel->getTopic() + END_OF_MESSAGE); } //332
std::string	RPL_INVITING(std::string const &channel_name, std::string const &nick) { return (channel_name + " " + nick + END_OF_MESSAGE); } //341
std::string	RPL_NAMREPLY(Channel *channel) { return (channel->getName() + " :" + channel->getUsersList() + END_OF_MESSAGE); } //353
std::string	RPL_ENDOFNAMES(Channel *channel) { return (channel->getName() + " :End of NAMES list" + END_OF_MESSAGE); } //366
std::string	RPL_YOUREOPER(void) { return (":You are now an IRC operator\r\n"); } //381

/* 400 - 599 */
std::string	ERR_NOSUCHNICK(std::string const &nick_or_channel_name) { return (nick_or_channel_name + " :No such nick/channel" + END_OF_MESSAGE); } //401
std::string	ERR_NOSUCHCHANNEL(std::string const &channel_name) { return (channel_name); } //403
std::string	ERR_CANNOTSENDTOCHAN(std::string const &channel_name) { return (channel_name + " :Cannot send to channel" + END_OF_MESSAGE); } //404
std::string	ERR_TOOMANYCHANNELS(std::string const &channel_name) { return (channel_name + "<channel name> :You have joined too many channels"); } //405
std::string	ERR_NORECIPIENT(std::string const &command) { return (":No recipient given (" + command + ")" + END_OF_MESSAGE); } //411
std::string	ERR_NOTEXTTOSEND(void) { return (":No text to send\r\n"); } //412
std::string	ERR_NONICKNAMEGIVEN(void) { return (":No nickname given\r\n"); } //431
std::string	ERR_ERRONEUSNICKNAME(std::string const &nick) { return (nick + " :Erroneous nickname" + END_OF_MESSAGE); } //432
std::string	ERR_NICKNAMEINUSE(std::string const &nick) { return (nick + " :Nickname is already in use" + END_OF_MESSAGE); } //433
std::string	ERR_UNAVAILRESOURCE(std::string const &channel_name, std::string const &nick) { return (channel_name + nick + "<nick/channel> :Nick/channel is temporarily unavailable"); } //437
std::string	ERR_USERNOTINCHANNEL(std::string const &nick, std::string const &channel_name) { return (nick + " " + channel_name + " :They aren't on that channel" + END_OF_MESSAGE); } //441
std::string	ERR_NOTONCHANNEL(std::string const &channel_name) { return (channel_name + " :You're not on that channel" + END_OF_MESSAGE); } //442
std::string	ERR_USERONCHANNEL(std::string const &nick, std::string const &channel_name) { return (nick + " " + channel_name + " :is already on channel" + END_OF_MESSAGE); } //443
std::string	ERR_NEEDMOREPARAMS(std::string const &cmd) { return ("Command " + cmd + " :Not enough parameters" + END_OF_MESSAGE); } //461
std::string	ERR_ALREADYREGISTRED(void) { return (":Unauthorized command (already registered)\r\n"); } //462
std::string	ERR_PASSWDMISMATCH(void) { return (":Password incorrect\r\n"); } //464
std::string	ERR_CHANNELISFULL(std::string const &channel_name) { return (channel_name + "<channel> :Cannot join channel (+l)"); } //471
std::string	ERR_UNKNOWNMODE(std::string const &mode, std::string const &channel_name) { return (mode + " :is unknown mode char to me for " + channel_name + END_OF_MESSAGE); }//472
std::string	ERR_INVITEONLYCHAN(std::string const &channel_name) { return (channel_name + " :Cannot join channel (+i)" + END_OF_MESSAGE); } //473
std::string	ERR_BANNEDFROMCHAN(std::string const &channel_name) { return (channel_name + "<channel> :Cannot join channel (+b)"); } //474
std::string	ERR_BADCHANNELKEY(std::string const &channel_name) { return (channel_name + " :Cannot join channel (+k)" + END_OF_MESSAGE); } //475
std::string	ERR_NOCHANMODES(std::string const &channel_name) { return (channel_name + " :Channel doesn't support modes"); } //477
std::string	ERR_BADCHANMASK(std::string const &channel_name) { return (channel_name + "<channel> :Bad Channel Mask"); } //476
std::string	ERR_NOPRIVILEGES(void) { return (":Permission Denied- You're not an IRC operator\r\n"); }// 481
std::string	ERR_CHANOPRIVSNEEDED(std::string const &channel_name) { return (channel_name + " :You're not channel operator" + END_OF_MESSAGE); } //482
std::string	ERR_CANTKILLSERVER(std::string const &server) { return (server + " :You can't kill a server!" + END_OF_MESSAGE); }// 483
std::string	ERR_UMODEUNKNOWNFLAG(void) { return (":Unknown MODE flag\r\n"); } //501
std::string	ERR_USERSDONTMATCH(void) { return (":Cannot change mode for other users\r\n"); } //502
/* 1000 - XXXX CUSTOM_REPLIES */
std::string	RPL_CHANNEL_CMD(std::string const &channel_cmd_list) { return ("Available command on this channel: " + channel_cmd_list); } // 1002

/* MOTD */
std::string	RPL_MOTDSTART(void) { return (":- Message of the day - \r\n"); } //375
std::string	RPL_MOTD(std::string const &line) { return (":" + line + END_OF_MESSAGE); } //372
std::string	RPL_ENDOFMOTD(void) { return (":End of MOTD command\r\n"); } //376
