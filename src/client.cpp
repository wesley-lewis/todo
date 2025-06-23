#include <sqlite3.h>
#include <string>
#include <fmt/format.h>
#include <iostream>
#include "../include/command.h"
#include "../include/client.h"

int listCallback(void* p, int argc, char** argv, char** colNames)
{
	for(int i = 0; i < argc; ++i)
	{
		std::cout << colNames[i] << " = " << (argv[i] ? argv[i]: "NULL") << '\n';
	}
	std::cout << "-------------------------\n";
	return 0;
}


void handleError(char* errMsg, const std::string& command) {
	std::cerr << "ERROR: Unable to execute " << command << " command\n";
	std::cerr << "ERROR: Msg: " << errMsg << '\n';
	sqlite3_free(errMsg);
}

int Client::runCommand(Command command)
{
	switch (command.getCommandType()) {
		case CommandType::List:
			{
				std::cout << "Working on getting the list\n";
				const std::string sql = "SELECT * from tasks;";
				char *errMsg = nullptr;
				int rc = sqlite3_exec(db, sql.c_str(), listCallback, 0, &errMsg);
				if (rc) {
					handleError(errMsg, "list");
					return 1;
				}
				break;
			}
		case CommandType::Insert:
			{
				const std::string sql = fmt::format("INSERT INTO tasks (task, completed) VALUES('{}', {}) ", command.getData(), 0);
				char *errMsg = nullptr;
				int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
				if (rc) {
					handleError(errMsg, "create");
					return 1;
				}
				break;
			}
		case CommandType::Delete:
			{
				const std::string sql = fmt::format("DELETE FROM tasks WHERE id={};", command.getData() );
				char *errMsg = nullptr;
				int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
				if (rc) {
					handleError(errMsg, "delete");
					return 1;
				}
				break;
			}
		case CommandType::Complete:
			{
				// TODO: 
				// Create the SQL query
				// Modify the value with id=<data>
				const std::string& sql = fmt::format("UPDATE tasks SET completed=1 WHERE id={}", command.getData());
				char *errMsg = nullptr;
				int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
				if (rc) {
					handleError(errMsg, "complete");
					return 1;
				}
				break;
			}
	}
	return 0;
}
