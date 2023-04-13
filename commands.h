#pragma once
#include <vector>
#include <string>

#include <iostream>

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
	DIR,
	STR_REST
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
	const void (*func)(const CommandArg* argv);
	const int argc;
	CommandArgType* argTypes;
};

class PreparedCommand
{
	public:
		const void (*func)(const CommandArg* argv);
		const CommandArg* argv = nullptr;
		PreparedCommand(const void (*func)(const CommandArg* argv), const CommandArg* argv);
		~PreparedCommand();
		PreparedCommand(PreparedCommand& obj);
		PreparedCommand(const PreparedCommand& obj);
};

class CommandsModule
{
	private:
		std::vector<Command> commandList;
		std::vector<std::string> splitCommand(std::string command);
		CommandArg* getCommandArgs(std::vector<std::string> args, const CommandArgType* argTypes, const int argc);
	public:   
		CommandsModule();
		~CommandsModule();
		void addCommand(Command command);
		Command* lookupCommand(std::string name);
		PreparedCommand prepareCommand(std::string command);
		bool runCommand(std::string command);
		bool runCommand(PreparedCommand command);
};
