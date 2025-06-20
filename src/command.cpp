#include <iostream>
#include "../include/command.h"

CommandType Command::getCommandType() {
	return command_type;
}

void Command::printCommand() {
	switch (command_type) {
		case CommandType::Create:
			std::cout << "CommandType: Create, Data: " << data << '\n';
			break;
		case CommandType::Delete:
			std::cout << "CommandType: Delete, Data: " << data << '\n';
			break;
		case CommandType::List:
			std::cout << "CommandType: List\n";
			break;
	}
}
