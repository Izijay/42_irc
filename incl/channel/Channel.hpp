#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class User;
class Server;
class Bot;

#include "../irc.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"
#include "../bot/Bot.hpp"
#include "../utils.hpp"
#include <map>
#include <vector>
#include <string>

class Channel
{
	public:
		Channel(std::string const, std::string const, User*, std::string const);
		~Channel();

		void	acceptUser(User *);
		void	delUser(User *);
		void	sendToAllUser(std::string, std::string, User *, bool);
		void	sendListUser(User *);
		void	checkOper(void);

		std::map<int, User*> const	getUsers(void) const;
		std::string	const			getKey(void) const;
		std::string	const			getName(void) const;
		std::string	const			getTopic(void) const;
		std::string	const			getUsersList(void);
		User *						getCreator(void) const;
		std::map<int, User*>		getOperator(void) const;
		std::vector<char> const		getMode(void) const;
		std::vector<std::string>	getInviteList(void) const;
		bool						getInviteOnly(void) const;


		void	setTopic(std::string const);
		void	setMode(bool, char);
		void	setMode(bool, char, std::string const, User *, Server *);
		void	setInviteList(std::string const);

	private:
		Bot *						_bot;
		User *						_creator;
		std::map<int, User*>		_operator;
		std::map<int, User*>		_users;

		std::vector<char>			_mode;
		std::vector<char>			_available_mode;
		std::vector<std::string>	_invite_list;
		std::string					_topic;
		std::string					_name;
		std::string					_key;

		bool						_invite_only;
};

#endif

/* ONLY HANDLE THESE MODES */
	/* oik */

/* o - give/take channel operator privilege; */
/* i - toggle the invite-only channel flag; */
/* k - set/remove the channel key (password); */
