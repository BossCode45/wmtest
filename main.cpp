#include <iostream>
#include <map>

#include "commands.h"
#include "error.h"
#include "config.h"
#include "keybinds.h"

CommandsModule commandsModule;

using std::cout, std::endl, std::string;

int main(int argc, char** argv)
{
	std::string home = getenv("HOME");
	std::string pathAfterHome = "/.config/YATwm/config.toml";
	std::string file = home + pathAfterHome;

	Config cfg(commandsModule);
	KeybindsModule keys(commandsModule);

	cfg.loadFromFile("config");
	cout << cfg.gaps << " " << cfg.outerGaps << endl;
	for(Workspace w : cfg.workspaces)
	{
		cout << w.name;
		for(int i = 0; i < w.screenPreferencesc; i++)
		{
			cout << " " << w.screenPreferences[i];
		}
		cout << endl;
	}
	try
	{
		commandsModule.runCommand("readBinds");
	}
	catch (Err e)
	{
		cout << "Error: " << e.code << endl;
		cout << "\tMessage: " << e.message << endl;
	}
	return 0;
}
