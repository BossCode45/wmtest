#include "commands.h"
#include "error.h"

#include <cctype>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <regex>

using std::cout, std::endl, std::string, std::vector, std::tolower;

const void printHello(const int argc, const CommandArg* argv)
{
	cout << "Hello" << endl;
}

const void echo(const int argc, const CommandArg* argv)
{
	cout << "Echo: '" << argv[0].str << '\'' << endl;
}

std::map<string, PreparedCommand> binds;
const void bind(const int argc, const CommandArg* argv)
{
	string cmdstr = "";
}

CommandsModule::CommandsModule()
{
	const Command c = {"printHello", printHello, 0, {}};
	addCommand(c);
	CommandArgType* args = new CommandArgType[1];
	args[0] = STR;
	const Command c2 = {"echo", echo, 1, args};
	addCommand(c2);
}
CommandsModule::~CommandsModule()
{
}

void CommandsModule::addCommand(Command command)
{
	commandList.push_back(command);
}

struct NameMatches
{
	NameMatches(string s): s_{s} {}
	bool operator()(Command c) { return (c.name == s_); }
	string s_;        
};

Command* CommandsModule::lookupCommand(string name)
{
	auto elem = std::find_if(commandList.begin(), commandList.end(), NameMatches(name));
	if (elem != commandList.end())
	{
		int i = elem - commandList.begin();
		return &(commandList[i]);
	}
	else
	{
		return nullptr;
	}
}

vector<string> CommandsModule::splitCommand(string command)
{
	vector<string> v;
	string regexString = "((?:\"[^\"\n]+\")|(?:'[^'\n]+')|(?:[^ \n]+))+";
	std::regex regex(regexString, std::regex_constants::_S_icase);
	auto begin = std::sregex_iterator(command.begin(), command.end(), regex);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; i++)
	{
		std::smatch match = *i;
		std::string str = match.str();
		if(str[0] == '\'' || str[0] == '"')
			str = str.substr(1, str.size() - 2);
		v.push_back(str);
	}
	return v;
}

template <typename T>
std::basic_string<T> lowercase(const std::basic_string<T>& s)
{
    std::basic_string<T> s2 = s;
    std::transform(s2.begin(), s2.end(), s2.begin(), [](unsigned char c){ return std::tolower(c); });
    return s2;
}

CommandArg* getCommandArgs(vector<string> split, const CommandArgType* argTypes)
{
	CommandArg* args = new CommandArg[split.size() -1];
	for(int i = 1; i < split.size(); i++)
	{
		switch(argTypes[i-1])
		{
			case STR: args[i-1].str = (char*)split[i].c_str(); break;
			case NUM: args[i-1].num = std::stoi(split[i]); break;
			case DIR: args[i-1].dir = LEFT; break;
			default: cout << "UH OH SOMETHING IS VERY WRONG";
		}
	}
	return args;
}

PreparedCommand CommandsModule::prepareCommand(string command)
{
	vector<string> split = splitCommand(command);
	Command* cmd = lookupCommand(split[0]);
	if(cmd == nullptr)
		throw Err(CMD_ERR_NOT_FOUND, split[0] + " is not a valid command name");
	if(cmd->argc > split.size())
		throw Err(CMD_ERR_WRONG_ARGS, command + " is the wrong args");
	CommandArg* args = ::getCommandArgs(split, cmd->argTypes);
	return PreparedCommand(cmd->func, (int) (split.size() - 1), args);
}

bool CommandsModule::runCommand(string command)
{
	vector<string> split = splitCommand(command);
	Command* cmd = lookupCommand(split[0]);
	if(cmd == nullptr)
		throw Err(CMD_ERR_NOT_FOUND, split[0] + " is not a valid command name");
	if(cmd->argc > split.size())
		throw Err(CMD_ERR_WRONG_ARGS, command + " is the wrong args");
	CommandArg* args = ::getCommandArgs(split, cmd->argTypes);
	cmd->func(split.size() - 1, args);
	delete[] args;
	return true;
}

bool CommandsModule::runCommand(PreparedCommand command)
{
	command.func(command.argc, command.argv);
	return true;
}

PreparedCommand::PreparedCommand(const void (*func)(const int argc, const CommandArg* argv), int argc, const CommandArg* argv)
{
	this->func = func;
	this->argc = argc;
	this->argv = argv;
}
PreparedCommand::~PreparedCommand()
{ 
	if(argv != nullptr)
		delete[] argv;
}
PreparedCommand::PreparedCommand(PreparedCommand & obj)
{
	func = obj.func;
	argc = obj.argc;
	argv = obj.argv;
	obj.argv = nullptr;
}
