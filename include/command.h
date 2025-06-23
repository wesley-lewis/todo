#ifndef COMMAND_H
#define COMMAND_H

#pragma once

#include <string>

enum CommandType {
	Insert,
	List,
	Complete,
	Delete,
	ListCompleted,
};

class Command {
	private:
		CommandType command_type;
		const std::string& data;

	public:
		Command(CommandType command_type, const std::string& data) : command_type(command_type), data(data) {}

		CommandType getCommandType(); 

		const std::string& getData() {
			return data;
		}

		void printCommand();
};

#endif
