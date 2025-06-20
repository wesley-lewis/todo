#ifndef CLIENT_H
#define CLIENT_H

#pragma once
#include <string>
#include <sqlite3.h>
#include "../include/command.h"

class Client {
	private:
		sqlite3 *db;
		const std::string& filepath;

	public:
		Client(sqlite3 *db, const std::string& filepath) : db(db), filepath(filepath) {}

		int runCommand(Command command);

};

#endif
