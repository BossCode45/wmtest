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

#define KEYCOM(X) \
	void X (const KeyArg arg)
KEYCOM(exit);
KEYCOM(spawn);
KEYCOM(changeWS);
KEYCOM(wToWS);
KEYCOM(focChange);
KEYCOM(reload);

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
		int numWS;
		std::string* workspaceNames;
		int workspaceNamesc;
		int maxMonitors;
		int** screenPreferences;
		int screenPreferencesc;

		// Keybinds
		KeyBind* binds;
		int bindsc;
	private:
		CommandsModule& commandsModule;
		bool loaded = false;
};
