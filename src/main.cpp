#include <sqlite3.h>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include "../include/command.h"
#include "../include/client.h"

/*
 * Objectives:
 * - Add a task
 * - Complete a task
 * - Remove a task
 * - Remove all completed tasks
 *
 * FUTURE:
 * - Remove all tasks that contain a particular keyword
*/

/// convert '~' to the user directory of the current user
/// converts ~/.local/share/todo -> /home/user/.local/share/todo
const std::string expand_home(const std::string& path) 
{
	if (path[0] == '~') {
		const char* home = std::getenv("HOME");
		if (home) {
			const std::string homepath(home);
			const std::string filepath = homepath + path.substr(1);
			return filepath;
		}
	}
	return path;
}

int create_dir(const std::string &path)
{
	if (std::filesystem::create_directory(path)) {
		return 1; 
	}
	return 0;
}

int createTasksTable(sqlite3 *db)
{
	char *errMsg;
	if (db == nullptr) {
		std::cerr << "ERROR: Db is not initialized\n";
		return 1;
	}
	const char* sql = "CREATE TABLE IF NOT EXISTS tasks (id INTEGER PRIMARY KEY, task TEXT, completed INTEGER);";
	int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << errMsg << '\n';
		sqlite3_free(errMsg);
		return 1;
	}
	return 0;
}

int execSql(sqlite3 *db, const char* sql, int (*callback)(void *, int, char **, char **))
{
	char *errMsg = nullptr;
	if (db == nullptr) {
		std::cerr << "ERROR: Db is uninitialized\n";
		return 1;
	}
	int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
	if (rc != SQLITE_OK) {
		std::cerr << "ERROR: while executing sql: " << errMsg << '\n';
		sqlite3_free(errMsg);
		return 1;
	}
	return 0;
}

int createConnection(const std::string &filepath, sqlite3 **db)
{
	int rc = sqlite3_open(filepath.c_str(), db);
	if (rc) {
		std::cerr << "ERROR: Unable to open database: " << '\n';
		return 1;
	}
	return 0;
}

int printSelect(void* arg, int argc, char** argv, char** colNames)
{
	for(int i = 0; i < argc; ++i)
	{
		std::cout << colNames[i] << " = " << (argv[i] ? argv[i]: "NULL") << '\n';
	}
	std::cout << "-------------------------\n";
	return 0;
}

void printUsage()
{
	std::cout << "Usage: \n";
	std::cout << "Create task: todo -c \"task1\"\n";
	std::cout << "List tasks(will list all the tasks) : todo -l\n";
	std::cout << "Delete task: todo -d <task-id>\n";
}

Command parseArgs(const std::string &cmd, const std::string &data)
{
	CommandType cmd_type;
	if (cmd == "-l") {
		cmd_type = CommandType::List;
	}else if (cmd == "-d") {
		cmd_type = CommandType::Delete;
	}else if (cmd == "-i") {
		cmd_type = CommandType::Insert;
	}else if (cmd == "-c") {
		cmd_type = CommandType::Complete;
	}else if (cmd == "-lc") {
		cmd_type = CommandType::ListCompleted;
	}else {
		std::cerr << "ERROR: Invalid command\n";
		std::exit(1);
	}
	Command command(cmd_type, data);
	return command;
}

int main(int argc, char **argv)
{
	sqlite3 *db;
	// the folder where the sqlite file will be stored
	const std::string default_folder = "~/.local/share/todo/";
	const std::string folder = expand_home(default_folder);
	if(create_dir(folder)) {
		std::cerr << "ERROR: Unable to create directory\n";
		return 1;
	}
	// the file where the db is stored
	const char* filename = "file1.sqlite";
	// the filepath where the .sqlite file will be stored.
	std::string filepath = folder + filename;

	// create the connection with sqlite
	int rc = createConnection(filepath, &db);
	if (rc) return rc;
  // std::cout << "IN_FO: connected to sqlite" << "\n";

	// My abstraction of sqlite3 client
	Client client(db, filepath);

	// No. Arguments should either be 2 or 3
	if (argc == 2) {
		// First argument should be -l
		const std::string cmd(argv[1]);
		const std::string data = "";
		Command command = parseArgs(cmd, data);
		client.runCommand(command);
	}else if (argc == 3) {
		// First arugment should be either -c or -d
		const std::string cmd(argv[1]);
		// The data to be inserted or the key to be deleted
		const std::string data(argv[2]);
		Command command = parseArgs(cmd, data);
		client.runCommand(command); // run the command
	}else {
		printUsage();
	}

	return 0;
}
