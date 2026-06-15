#include "DataBase.h"

// Constructor: Opens the database connection
Database::Database(const std::string& databaseName) {
    dbName = databaseName;
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}

// Destructor: Closes the database connection
Database::~Database() {
    sqlite3_close(db);
}

// Helper function to execute SQL queries without expecting a result back
void Database::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Creates necessary tables if they don't exist already
void Database::initializeDatabase() {
    executeQuery("CREATE TABLE IF NOT EXISTS Players (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE);");
    executeQuery("CREATE TABLE IF NOT EXISTS Saved_Monsters (id INTEGER PRIMARY KEY AUTOINCREMENT, player_name TEXT, name TEXT, hp INTEGER, attack INTEGER);");
    executeQuery("CREATE TABLE IF NOT EXISTS Saved_Items (id INTEGER PRIMARY KEY AUTOINCREMENT, player_name TEXT, name TEXT);");
    executeQuery("CREATE TABLE IF NOT EXISTS Stat_Monster_Usage (player_name TEXT, monster_name TEXT, times_used INTEGER, PRIMARY KEY(player_name, monster_name));");
    executeQuery("CREATE TABLE IF NOT EXISTS Stat_Item_Usage (player_name TEXT, item_name TEXT, times_used INTEGER, kills INTEGER, PRIMARY KEY(player_name, item_name));");
    executeQuery("CREATE TABLE IF NOT EXISTS Global_Stats (id INTEGER PRIMARY KEY, total_kills INTEGER);");
    executeQuery("INSERT OR IGNORE INTO Global_Stats (id, total_kills) VALUES (1, 0);");
}

// Saves the character's current state (Save Game)
void Database::saveCharacter(Character& player) {
    std::string pName = player.getName(); 
    
    // Create player if they do not exist
    std::string query = "INSERT OR IGNORE INTO Players (name) VALUES ('" + pName + "');";
    executeQuery(query);
    
    // Clear old save-data for this player to prevent duplicates
    executeQuery("DELETE FROM Saved_Monsters WHERE player_name = '" + pName + "';");
    executeQuery("DELETE FROM Saved_Items WHERE player_name = '" + pName + "';");
    
    // Save Monsters
    for (const auto& m : player.getMonsters()) {
        query = "INSERT INTO Saved_Monsters (player_name, name, hp, attack) VALUES ('" 
              + pName + "', '" + m.getName() + "', " + std::to_string(m.getHealth()) + ", " + std::to_string(m.getAttackPower()) + ");";
        executeQuery(query);
    }

    // Save Items
    for (const auto& i : player.getItems()) {
        query = "INSERT INTO Saved_Items (player_name, name) VALUES ('" + pName + "', '" + i.getName() + "');";
        executeQuery(query);
    }
    
    std::cout << "\n[Database] Game saved successfully for " << pName << "!" << std::endl;
}

// Get all player names for loading purposes
std::vector<std::string> Database::getAllPlayerNames() {
    std::vector<std::string> names;
    sqlite3_stmt* stmt;
    std::string query = "SELECT name FROM Players;";
    
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            names.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    }
    sqlite3_finalize(stmt);
    return names;
}

// Load-function (Retrieves character and their monsters)
Character* Database::loadCharacter(const std::string& playerName) {
    sqlite3_stmt* stmt;
    std::string query = "SELECT name FROM Players WHERE name = '" + playerName + "';";
    bool exists = false;
    
    // Check if the player exists in the database
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) exists = true;
    }
    sqlite3_finalize(stmt);

    if (!exists) return nullptr;

    Character* loadedPlayer = new Character(playerName);
    
    // LOAD MONSTERS FROM DATABASE
    std::string qMonsters = "SELECT name, hp, attack FROM Saved_Monsters WHERE player_name = '" + playerName + "';";
    if (sqlite3_prepare_v2(db, qMonsters.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string mName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int mHp = sqlite3_column_int(stmt, 1);
            int mAttack = sqlite3_column_int(stmt, 2);
            
            // Recreate the monster and add it to the player's squad
            Monster loadedMonster(mName, mHp, mAttack);
            loadedPlayer->addMonster(loadedMonster);
        }
    }
    sqlite3_finalize(stmt);

    // LOAD ITEMS FROM DATABASE
    std::string qItems = "SELECT name FROM Saved_Items WHERE player_name = '" + playerName + "';";
    if (sqlite3_prepare_v2(db, qItems.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string iName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            
            // Create a baseline item
            Item loadedItem(iName, 0, StatusType::NONE, 0, 0);

            // Restore exact stats based on the item name
            if (iName == "Health Potion") loadedItem = Item("Health Potion", -15, StatusType::NONE, 0, 0);
            else if (iName == "Poison Dagger") loadedItem = Item("Poison Dagger", 2, StatusType::POISON, 3, 100);
            else if (iName == "Ice Bomb") loadedItem = Item("Ice Bomb", 1, StatusType::FROZEN, 1, 100);
            else if (iName == "Smoke Bomb") loadedItem = Item("Smoke Bomb", 0, StatusType::CONFUSED, 3, 80);
            else if (iName == "Jagged Spear") loadedItem = Item("Jagged Spear", 4, StatusType::BLEEDING, 3, 100);
            else if (iName == "Rage Elixir") loadedItem = Item("Rage Elixir", 0, StatusType::STRENGTH_BUFF, 3, 100);
            else if (iName == "Iron Shield") loadedItem = Item("Iron Shield", 0, StatusType::DEFENSE_BUFF, 5, 100);
            else if (iName == "Adrenaline Shot") loadedItem = Item("Adrenaline Shot", 0, StatusType::SPEED_BUFF, 4, 100);

            // Put the recreated item back into the player's inventory
            loadedPlayer->addItem(loadedItem);
        }
    }
    sqlite3_finalize(stmt);
    
    std::cout << "\n[Database] " << playerName << " has been loaded with their squad!" << std::endl;
    return loadedPlayer;
}

// statistics tracking functions
void Database::incrementGlobalKills() { executeQuery("UPDATE Global_Stats SET total_kills = total_kills + 1 WHERE id = 1;"); }
void Database::incrementMonsterUsage(const std::string& pName, const std::string& mName) {
    std::string query = "INSERT INTO Stat_Monster_Usage (player_name, monster_name, times_used) VALUES ('" + pName + "', '" + mName + "', 1) "
                        "ON CONFLICT(player_name, monster_name) DO UPDATE SET times_used = times_used + 1;";
    executeQuery(query);
}
void Database::incrementItemUsage(const std::string& pName, const std::string& iName, bool resultedInKill) {
    int k = resultedInKill ? 1 : 0;
    std::string query = "INSERT INTO Stat_Item_Usage (player_name, item_name, times_used, kills) VALUES ('" + pName + "', '" + iName + "', 1, " + std::to_string(k) + ") "
                        "ON CONFLICT(player_name, item_name) DO UPDATE SET times_used = times_used + 1, kills = kills + " + std::to_string(k) + ";";
    executeQuery(query);
}

void Database::printGameStatistics() {
    std::cout << "\n========== GAME STATISTICS ==========" << std::endl;
    sqlite3_stmt* stmt;

    // 1. Total monsters killed in the game
    if (sqlite3_prepare_v2(db, "SELECT total_kills FROM Global_Stats WHERE id = 1;", -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Total monsters killed in the game: " << sqlite3_column_int(stmt, 0) << std::endl;
        }
    }
    sqlite3_finalize(stmt);

    // 2. Most used monster per player
    std::cout << "\n--- Most used monster per player ---" << std::endl;
    std::string qMonster = "SELECT player_name, monster_name, MAX(times_used) FROM Stat_Monster_Usage GROUP BY player_name;";
    if (sqlite3_prepare_v2(db, qMonster.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_text(stmt, 0) << " mostly uses: " 
                      << sqlite3_column_text(stmt, 1) << " (" << sqlite3_column_int(stmt, 2) << " times)" << std::endl;
        }
    }
    sqlite3_finalize(stmt);

    // 3. Most used item per player
    std::cout << "\n--- Most used item per player ---" << std::endl;
    std::string qItemUse = "SELECT player_name, item_name, MAX(times_used) FROM Stat_Item_Usage GROUP BY player_name;";
    if (sqlite3_prepare_v2(db, qItemUse.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_text(stmt, 0) << " mostly uses: " 
                      << sqlite3_column_text(stmt, 1) << " (" << sqlite3_column_int(stmt, 2) << " times)" << std::endl;
        }
    }
    sqlite3_finalize(stmt);

    // 4. Kills by each item
    std::cout << "\n--- Kills by each item ---" << std::endl;
    std::string qItemKills = "SELECT item_name, SUM(kills) FROM Stat_Item_Usage WHERE kills > 0 GROUP BY item_name;";
    if (sqlite3_prepare_v2(db, qItemKills.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_text(stmt, 0) << " has killed: " 
                      << sqlite3_column_int(stmt, 1) << " monsters" << std::endl;
        }
    }
    sqlite3_finalize(stmt);

    std::cout << "=====================================\n" << std::endl;
}