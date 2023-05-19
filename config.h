#pragma once

#include "commands.h"
#include <X11/X.h>
#include <X11/keysym.h>

#include <string>

typedef union
{
	char* str;
	int num;
	MoveDir dir;
} KeyArg;

struct KeyBind
{
	unsigned int modifiers;
	KeySym keysym;
	void(* func) (const KeyArg arg);
	KeyArg args;
};

struct Workspace
{
	std::string name;
	int* screenPreferences;
	int screenPreferencesc;
};

#define COMMAND(X) \
	const void X (const CommandArg* argv)

class Config
{  
	public:
		Config(CommandsModule& commandsModule);
		~Config();
		void free();
	
		void loadFromFile(std::string path);
		// Startup
		std::string* startupBash;
		int startupBashc;

		// Main
		int gaps;
		int outerGaps;
		std::string logFile;

		// Workspaces
		std::vector<Workspace> workspaces;
		int numWS;
		std::string* workspaceNames;
		int workspaceNamesc;
		int maxMonitors;
		int** screenPreferences;
		int screenPreferencesc;

		// Keybinds
		KeyBind* binds;
		int bindsc;

		// Config Commands
		COMMAND(gapsCmd);
		COMMAND(outerGapsCmd);
		COMMAND(logFileCmd);
		COMMAND(addWorkspaceCmd);

		// Keybind Commands
		COMMAND(exit);
		COMMAND(spawn);
		COMMAND(changeWS);
		COMMAND(wToWS);
		COMMAND(focChange);
		COMMAND(reload);
	private:
		CommandsModule& commandsModule;
		bool loaded = false;
};
