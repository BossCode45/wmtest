#include "config.h"
#include "commands.h"

#include <X11/Xlib.h>

#include <cstdio>
#include <fstream>
#include <ios>
#include <string>
#include <vector>
#include <sstream>

//Just for testing
#include <iostream>

using std::string;

// For testing
using std::cout, std::endl;

const void Config::exit(const CommandArg* argv)
{
	cout << "exit called" << endl;
}
const void Config::spawn(const CommandArg* argv)
{
	cout << "spawn called " << argv[0].str << endl;
}
const void Config::changeWS(const CommandArg* argv)
{
	cout << "changeWS called" << endl;
}
const void Config::wToWS(const CommandArg* argv)
{
	cout << "wToWS called" << endl;
}
const void Config::focChange(const CommandArg* argv)
{
	cout << "focChange called" << endl;
}
const void Config::reload(const CommandArg* argv)
{
	cout << "reload called" << endl;
}

const void Config::gapsCmd(const CommandArg* argv)
{
	gaps = argv[0].num;
}

const void Config::outerGapsCmd(const CommandArg* argv)
{
	outerGaps = argv[0].num;
}

const void Config::logFileCmd(const CommandArg* argv)
{
	logFile = argv[0].str;
}

Config::Config(CommandsModule& commandsModule)
	: commandsModule(commandsModule)
{
	//Register commands for keybinds
	CommandArgType* spawnArgs = new CommandArgType[1];
	spawnArgs[0] = STR;
	commandsModule.addCommand("spawn", &Config::spawn, 1, spawnArgs, this);

	//Register commands for config
	CommandArgType* gapsArgs = new CommandArgType[1];
	gapsArgs[0] = NUM;
	commandsModule.addCommand("gaps", &Config::gapsCmd, 1, gapsArgs, this);
	commandsModule.addCommand("outergaps", &Config::outerGapsCmd, 1, gapsArgs, this);
	CommandArgType* logFileArgs = new CommandArgType[1];
	logFileArgs[0] = STR_REST;
	commandsModule.addCommand("logfile", &Config::logFileCmd, 1, logFileArgs, this);

	//Set defaults
	gaps = 3;
	outerGaps = 3;
	logFile = "/tmp/yatlog.txt";
	numWS = 10;
}


void Config::loadFromFile(string path)
{
	string cmd;
	int line = 0;
	std::ifstream config(path);
	while(getline(config, cmd))
	{
		if(cmd.at(0) == '#')
			continue;
		try
		{
			commandsModule.runCommand(cmd);
		}
		catch (Err e)
		{
			cout << "Error in config (line " << line <<  "): " << e.code << endl;
			cout << "\tMessage: " << e.message << endl;
		}
		line++;
	}
}

Config::~Config()
{
	free();
}
void Config::free()
{
	if(!loaded)
		return;
	delete[] startupBash;
	delete[] workspaceNames;
	for(int i = 0; i < screenPreferencesc; i++)
	{
		delete[] screenPreferences[i];
	}
	delete[] screenPreferences;
}
