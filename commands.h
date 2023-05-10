#pragma once

#include "error.h"

#include <vector>
#include <string>
#include <any>
#include <functional>

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
	const std::function<void(std::any&, const CommandArg* argv)> func;
	const int argc;
	CommandArgType* argTypes;
	std::any* module;
};
class CommandsModule
{
	private:
		std::vector<Command> commandList;
		std::vector<std::string> splitCommand(std::string command);
		CommandArg* getCommandArgs(std::vector<std::string>& args, const CommandArgType* argTypes, const int argc);
		const void printHello(const CommandArg* argv);
		const void echo(const CommandArg* argv);
	public:   
		CommandsModule();
		~CommandsModule();
		template <class T>
		void addCommand(std::string name, const void(T::*func)(const CommandArg*), const int argc, CommandArgType* argTypes, T* module);
		void addCommand(Command c);
		Command* lookupCommand(std::string name);
		void runCommand(std::string command);
		Err checkCommand(std::string command);
};

// YES I KNOW THIS IS BAD
// but it needs to be done this way
template <class T>
void CommandsModule::addCommand(std::string name, const void(T::*func)(const CommandArg*), const int argc, CommandArgType* argTypes, T* module)
{
	Command c = {name, (const void*(std::any::*)(const CommandArg* argv)) func, argc, argTypes, (std::any*)module};
	commandList.push_back(c);
}
