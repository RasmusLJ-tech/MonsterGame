#include "Dungeon.h"
#include <stdlib.h> // For rand()

Dungeon::Dungeon(std::string name, int playerTeamLevel, int difficultySetting) : name(name) {
    // Diffucluty scaling: The higher the difficulty, the stronger and more numerous the monsters in the dungeon
    
    // Number of enemies is directly tied to the difficulty setting, so harder dungeons have more monsters to fight through
    int numberOfEnemies = difficultySetting; // Easy = 1, Medium = 2, Hard = 3 number of enemies
    
    for (int i = 0; i < numberOfEnemies; ++i) {
        // Balanced stats multiplier based on player's current strength floor
        int scaleFactor = playerTeamLevel;
        
        int hp = 0;
        int attack = 0;

        // Balance configurations based on selected difficulty tier
        std::string monsterName = "Dungeon Goblin";
        if (difficultySetting == 1) {
            monsterName = "Weak Cave Imp";
            hp = (rand() % 3) + 3;          // Easy enemies get around 3-5 HP
            attack = 1;                     // Easy enemies deal exactly 1 damage
        }
        else if (difficultySetting == 2) {
            monsterName = "Fierce Orc Warrior";
            hp = (rand() % 4) + (scaleFactor + 2); 
            attack = (rand() % 2) + (scaleFactor / 3) + 1;
        }
        else if (difficultySetting == 3) {
            monsterName = "Grotto Overlord";
            hp = (rand() % 6) + (scaleFactor * 2);  
            attack = (rand() % 3) + (scaleFactor / 2) + 2;
        }

        if (attack < 1) attack = 1; // Safety boundary safeguard

        // Push the balanced dynamic combatant into the dungeon queue vector
        enemies.push_back(Monster(monsterName + " #" + std::to_string(i + 1), hp, attack));
    }
}

std::string Dungeon::getName() const { return name; }
std::vector<Monster>& Dungeon::getEnemies() { return enemies; }