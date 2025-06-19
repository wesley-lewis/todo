#include <string>
#include <iostream>

enum CommandType {
	Create,
	List,
	Delete,
};

class Command {
	private:
		CommandType command_type;
		const std::string &data;

	public:
		Command(CommandType command_type, const std::string &data) : command_type(command_type), data(data) {}

		CommandType getCommandType() {
			return command_type;
		}

		const std::string& getData() {
			return data;
		}

		void printCommand() {
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
};
