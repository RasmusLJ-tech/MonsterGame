#include "Dungeon.h"
#include <stdlib.h> // For rand()

Dungeon::Dungeon(std::string name, int playerTeamLevel, int difficultySetting) : name(name) {
    // difficultySetting: 1 = Easy, 2 = Medium, 3 = Hard
    
    // Bestem hvor mange monstre der er i denne grotte baseret på sværhedsgrad
    int numberOfEnemies = difficultySetting; // Easy = 1, Medium = 2, Hard = 3 monstre i træk
    
    for (int i = 0; i < numberOfEnemies; ++i) {
        // Skaler monstrenes stats dynamisk efter spillerens niveau
        int scaleFactor = playerTeamLevel * difficultySetting;
        
        int hp = (rand() % 5) + (scaleFactor * 2);      // Flere HP jo sværere
        int attack = (rand() % 2) + (scaleFactor / 2) + 1; // Mere skade jo sværere
        if (attack < 1) attack = 1;

        std::string monsterName = "Dungeon Goblin";
        if (difficultySetting == 1) monsterName = "Weak Cave Imp";
        if (difficultySetting == 2) monsterName = "Fierce Orc";
        if (difficultySetting == 3) monsterName = "Grotto Overlord";

        // Tilføj det eskalerede monster til grotten
        enemies.push_back(Monster(monsterName + " #" + std::to_string(i + 1), hp, attack));
    }
}

std::string Dungeon::getName() const { return name; }
std::vector<Monster>& Dungeon::getEnemies() { return enemies; }