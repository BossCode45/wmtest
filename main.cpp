#include <iostream>
#include <map>

#include "commands.h"
#include "error.h"
//#include "config.h"

using std::cout, std::endl, std::cerr, std::string, std::vector;

int main(int argc, char** argv)
{
	std::string home = getenv("HOME");
	std::string pathAfterHome = "/.config/YATwm/config.toml";
	std::string file = home + pathAfterHome;

	//Config cfg;

	CommandsModule commandsModule;
	try
	{
		commandsModule.runCommand("printHello");
		commandsModule.runCommand("echo test");
		commandsModule.runCommand("echo 'test 2'");
		PreparedCommand c = commandsModule.prepareCommand("echo 'test 3 wooah' extra");
		commandsModule.runCommand(c);
		commandsModule.runCommand("fsd;kljf");
	}
	catch (Err e)
	{
		cout << "Error: " << e.code << endl;
		cout << "\tMessage: " << e.message << endl;
	}
	return 0;
}
