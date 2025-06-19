#include <sqlite3.h>
#include <string>
#include <format>
#include <iostream>
#include "command.cpp"

int listCallback(void* p, int argc, char** argv, char** colNames)
{
	for(int i = 0; i < argc; ++i)
	{
		std::cout << colNames[i] << " = " << (argv[i] ? argv[i]: "NULL") << '\n';
	}
	std::cout << "-------------------------\n";
	return 0;
}

class Client {
	private:
		sqlite3 *db;
		const std::string &filepath;

	public:
		Client(sqlite3 *db, const std::string &filepath) : db(db), filepath(filepath) {}

		int runCommand(Command command)
		{
			switch (command.getCommandType()) {
				case CommandType::List:
					{
						const std::string sql = "SELECT * from tasks;";
						char *errMsg = nullptr;
						int rc = sqlite3_exec(db, sql.c_str(), listCallback, 0, &errMsg);
						if (rc) {
							std::cerr << "ERROR: Unable to execute list command\n";
							std::cerr << "ERROR: Msg: " << errMsg << '\n';
							sqlite3_free(errMsg);
							return 1;
						}
						break;
					}
				case CommandType::Create:
					{
						const std::string sql = std::format("INSERT INTO tasks (task, completed) VALUES({}, {}) ", command.getData(), 0);
						char *errMsg = nullptr;
						int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
						if (rc) {
							std::cerr << "ERROR: Unable to execute list command\n";
							std::cerr << "ERROR: Msg: " << errMsg << '\n';
							sqlite3_free(errMsg);
							return 1;
						}
						break;
					}
				case CommandType::Delete:
					const std::string sql = std::format("INSERT INTO tasks (task, completed) VALUES({}, {}) ", command.getData(), 0);
					char *errMsg = nullptr;
					int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
					if (rc) {
						std::cerr << "ERROR: Unable to execute list command\n";
						std::cerr << "ERROR: Msg: " << errMsg << '\n';
						sqlite3_free(errMsg);
						return 1;
					}
					break;
			}
			return 0;
		}
};
