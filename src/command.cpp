#include <iostream>
#include "../include/command.h"

CommandType Command::getCommandType() {
	return command_type;
}

void Command::printCommand() {
	std::cout << "CommandType: ";
	switch (command_type) {
		case CommandType::Insert:
			std::cout << "Create, Data: " << data << '\n';
			break;
		case CommandType::Delete:
			std::cout << "Delete, Data: " << data << '\n';
			break;
		case CommandType::List:
			std::cout << "List\n";
			break;
		case CommandType::Complete:
			std::cout << "Complete, Data: " << data << '\n';
			break;
	}
}
