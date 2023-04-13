#include "commands.h"
#include "error.h"

#include <cctype>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <regex>

extern CommandsModule commandsModule;

using std::cout, std::endl, std::string, std::vector, std::tolower;

const void printHello(const CommandArg* argv)
{
	cout << "Hello" << endl;
}

const void echo(const CommandArg* argv)
{
	cout << "Echo: '" << argv[0].str << '\'' << endl;
}

std::map<string, PreparedCommand> binds;
const void bind(const CommandArg* argv)
{
	cout << argv[0].str << endl;
	cout << argv[1].str << endl;
	PreparedCommand cmd = commandsModule.prepareCommand(argv[1].str);
	binds.insert({argv[0].str, cmd});
}

bool exitNow = false;
const void exit(const CommandArg* argv)
{
	exitNow = true;
}

const void readBinds(const CommandArg* argv)
{
	cout << "Reading binds" << endl;
	while(exitNow == false)
	{
		string key;
		std::getline(std::cin, key);
		cout << key << endl;
		commandsModule.runCommand(binds.find(key)->second);
	}
}

CommandsModule::CommandsModule()
{
	addCommand({"printHello", printHello, 0, {}});
	CommandArgType* args0 = new CommandArgType[1];
	args0[0] = STR;
	addCommand({"echo", echo, 1, args0});
	CommandArgType* args1 = new CommandArgType[2];
	args1[0] = STR;
	args1[1] = STR;
	addCommand({"bind", bind, 2, args1});
	addCommand({"exit", exit, 0, {}});
	addCommand({"readBinds", readBinds, 0, {}});
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

CommandArg* CommandsModule::getCommandArgs(vector<string> split, const CommandArgType* argTypes, const int argc)
{
	CommandArg* args = new CommandArg[argc];
	for(int i = 1; i < argc + 1; i++)
	{
		switch(argTypes[i-1])
		{
			case STR: args[i-1].str = (char*)split[i].c_str(); break;
			case NUM: args[i-1].num = std::stoi(split[i]); break;
			case DIR: args[i-1].dir = LEFT; break;
			case STR_REST:
			{
				string rest = "";
				for(int j = i; j < split.size(); j++)
				{
					rest += split[j];
				}
				args[i-1].str = (char*)rest.c_str();
				return args;
			}
			default: cout << "UH OH SOMETHING IS VERY WRONG" << endl;
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
	CommandArg* args = getCommandArgs(split, cmd->argTypes, cmd->argc);
	return PreparedCommand(cmd->func, args);
}

bool CommandsModule::runCommand(string command)
{
	vector<string> split = splitCommand(command);
	Command* cmd = lookupCommand(split[0]);
	if(cmd == nullptr)
		throw Err(CMD_ERR_NOT_FOUND, split[0] + " is not a valid command name");
	if(cmd->argc > split.size())
		throw Err(CMD_ERR_WRONG_ARGS, command + " is the wrong args");
	CommandArg* args = getCommandArgs(split, cmd->argTypes, cmd->argc);
	if(split[0] == "bind")
	{
		cout << args[0].str << endl;
		cout << args[1].str << endl;
	}
	cmd->func(args);
	delete[] args;
	return true;
}

bool CommandsModule::runCommand(PreparedCommand command)
{
	command.func(command.argv);
	return true;
}

PreparedCommand::PreparedCommand(const void (*func)(const CommandArg* argv), const CommandArg* argv)
{
	this->func = func;
	this->argv = argv;
}
PreparedCommand::~PreparedCommand()
{ 
	if(argv != nullptr)
		delete[] argv;
}
PreparedCommand::PreparedCommand(const PreparedCommand& obj)
{
	func = obj.func;
	argv = obj.argv;
}
PreparedCommand::PreparedCommand(PreparedCommand& obj)
{
	func = obj.func;
	argv = obj.argv;
	obj.argv = nullptr;
}
