#ifndef BOT_HPP
#define BOT_HPP

#include "../utils.hpp"
#include <string>
#include <vector>
#include <fstream>

class Bot
{
	public:
		Bot();
		~Bot();

		bool						censoringMessage(std::string &);

		std::vector<std::string>	getBannedWord(void) const;
		std::string					getPrefix(void) const;
		bool						getStatus(void) const;

		void						setBannedWord(std::string const);
		void						setStatus(std::string);

	private:
		std::vector<std::string>	_banned_word;
		std::string					_censure_word;
		std::string					_prefix;

		bool						_status;

};

#endif
