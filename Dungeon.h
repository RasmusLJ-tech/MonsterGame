#ifndef DUNGEON_H
#define DUNGEON_H

#include <string>
#include <vector>
#include "Monster.h"
#include "Character.h"

class Dungeon {
public:
    // Generates a dungeon with a name
    Dungeon(std::string name, int playerTeamLevel, int difficultySetting);

    std::string getName() const;
    std::vector<Monster>& getEnemies();
    bool isCleared() const;

private:
    std::string name;
    std::vector<Monster> enemies;
};

#endif