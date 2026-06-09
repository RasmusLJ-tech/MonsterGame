#include "Dungeon.h"
#include <stdlib.h> // For rand()

Dungeon::Dungeon(std::string name, int playerTeamLevel, int difficultySetting) : name(name) {
    // Diffucluty scaling: The higher the difficulty, the stronger and more numerous the monsters in the dungeon
    
    // Number of enemies is directly tied to the difficulty setting, so harder dungeons have more monsters to fight through
    int numberOfEnemies = difficultySetting; // Easy = 1, Medium = 2, Hard = 3 number of enemies
    
    for (int i = 0; i < numberOfEnemies; ++i) {
        // scaleFactor is based on both the player's team level and the difficulty setting, so monsters get stronger as the player progresses and chooses harder dungeons
        int scaleFactor = playerTeamLevel * difficultySetting;
        
        int hp = (rand() % 5) + (scaleFactor * 2);      // Flere HP jo sværere
        int attack = (rand() % 2) + (scaleFactor / 2) + 1; // Mere skade jo sværere
        if (attack < 1) attack = 1;

        std::string monsterName = "Dungeon Goblin";
        if (difficultySetting == 1) monsterName = "Weak Cave Imp";
        if (difficultySetting == 2) monsterName = "Fierce Orc";
        if (difficultySetting == 3) monsterName = "Grotto Overlord";

        // Adding a unique identifier to the monster name to differentiate multiple monsters in the same dungeon
        enemies.push_back(Monster(monsterName + " #" + std::to_string(i + 1), hp, attack));
    }
}

std::string Dungeon::getName() const { return name; }
std::vector<Monster>& Dungeon::getEnemies() { return enemies; }