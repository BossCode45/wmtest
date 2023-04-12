#pragma once
#include <vector>
#include <string>

#include <iostream>
using std::cout, std::endl;

enum MoveDir
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum CommandArgType
{
	STR,
	NUM,
	DIR
};

typedef union
{
	char* str;
	int num;
	MoveDir dir;
} CommandArg;

struct Command
{
	const std::string name;
	const void (*func)(const int argc, const CommandArg* argv);
	const int argc;
	const CommandArgType* argTypes;
};

class PreparedCommand
{
	public:
		const void (*func)(const int argc, const CommandArg* argv);
		int argc = -1;
		const CommandArg* argv = nullptr;
		PreparedCommand(const void (*func)(const int argc, const CommandArg* argv), int argc, const CommandArg* argv);
		~PreparedCommand();
		PreparedCommand(const PreparedCommand & obj);
		PreparedCommand(PreparedCommand && obj);
};

class CommandsModule
{
	private:
		std::vector<Command> commandList;
		std::vector<std::string> splitCommand(std::string command);
		CommandArg* getCommandArgs(std::vector<std::string> args);
	public:   
		CommandsModule();
		~CommandsModule();
		void addCommand(Command command);
		Command* lookupCommand(std::string name);
		PreparedCommand prepareCommand(std::string command);
		bool runCommand(std::string command);
		bool runCommand(PreparedCommand command);
};
