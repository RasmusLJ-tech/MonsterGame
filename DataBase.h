#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>
#include "Character.h"

class Database {
private:
    sqlite3* db;
    std::string dbName;

    // Helper function to execute SQL queries
    void executeQuery(const std::string& query);

public:
    Database(const std::string& databaseName);
    ~Database();

    // Initializes the database tables if they don't exist
    void initializeDatabase();

    // Save & Load fuctions for Character data
    void saveCharacter(Character& player);
    Character* loadCharacter(const std::string& playerName);
    std::vector<std::string> getAllPlayerNames();

    // Statisctics tracking
    void incrementGlobalKills();
    void incrementItemUsage(const std::string& playerName, const std::string& itemName, bool resultedInKill);
    void incrementMonsterUsage(const std::string& playerName, const std::string& monsterName);

    // Printing statistics to console
    void printGameStatistics();
};

#endif