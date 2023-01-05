#include "../incl/irc.hpp"
#include "../incl/server/Server.hpp"

bool exec = true;

void	signal_handler(int signal)
{
	(void)signal;
	exec = false;
}

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Wrong number of arguments\n";
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		signal(SIGINT, signal_handler);
		Server server(av[1], av[2], &exec);
		server.init();
		while(exec)
			server.exec();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
