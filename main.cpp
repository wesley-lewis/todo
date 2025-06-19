#include <sqlite3.h>
#include <filesystem>
#include <iostream>
#include <cstdlib>

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

int main(int argc, char **argv)
{
	const std::string default_folder = "~/.local/share/todo/";
	const std::string folder = expand_home(default_folder);
	if(create_dir(folder)) {
		std::cerr << "ERROR: Unable to create directory\n";
		return 1;
	}

	sqlite3 *db;
	const char* filename = "file1.sqlite";
	std::string filepath = folder + filename;
	
	int rc = createConnection(filepath, &db);
	if (rc) return rc;
	std::cout << "INFO: connected to sqlite: rc" << rc << "\n";

	rc = createTasksTable(db);
	if (rc) return rc;

	std::cout << "Created a SQLITE table\n";

	// const char *insert_sql = "INSERT INTO tasks (task, completed) VALUES('Task1', 0);";
	// rc = execSql(db, insert_sql, nullptr);
	
	const char* select_sql = "SELECT * from tasks;";
	rc = execSql(db, select_sql, printSelect);

	sqlite3_close(db);
	return 0;
}
