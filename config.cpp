#include "config.h"

#include <X11/Xlib.h>

#include <string>
#include <map>
#include <vector>
#include <sstream>

//Just for testing
#include <iostream>

using std::map, std::string;

// For testing
using std::cout, std::endl, std::cerr;

void exit(const KeyArg arg)
{
	cout << "exit called" << endl;
}
void spawn(const KeyArg arg)
{
	cout << "spawn called " << arg.str << endl;
}
void changeWS(const KeyArg arg)
{
	cout << "changeWS called" << endl;
}
void wToWS(const KeyArg arg)
{
	cout << "wToWS called" << endl;
}
void focChange(const KeyArg arg)
{
	cout << "focChange called" << endl;
}
void reload(const KeyArg arg)
{
	cout << "reload called" << endl;
}

map<string, void(*) (const KeyArg arg)> funcNameMap = {
	{"exit", exit},
	{"spawn", spawn},
	{"changeWS", changeWS},
	{"wToWS", wToWS},
	{"focChange", focChange},
	{"reload", reload}
};


Config::Config()
{
}

std::vector<string> split (const string &s, char delim) {
	std::vector<string> result;
	std::stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

void Config::loadFromFile(string path)
{
}

Config::~Config()
{
	free();
}
void Config::free()
{
	delete[] startupBash;
	delete[] workspaceNames;
	for(int i = 0; i < screenPreferencesc; i++)
	{
		delete[] screenPreferences[i];
	}
	delete[] screenPreferences;
}