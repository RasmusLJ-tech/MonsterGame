#include "Character.h"
#include "Monster.h"

Character::Character(const std::string& name) : name(name) {} // Constructor to initialize the character's name

void Character::addMonster(const Monster& newMonster) { // Add a new monster to the character's collection (up to 4 monsters)
    if (monsters.size() < 4) {
        monsters.push_back(newMonster);
    }
}

void Character::replaceMonster(int index, const Monster& m) { // Replace a monster at a specific index (0-3)
    if (index >= 0 && index < monsters.size()) {
        monsters[index] = m;
    }
}

bool Character::hasAliveMonsters() const {
    for (const Monster& monster : monsters) { // Check if any monster is alive
        if (monster.isAlive()) {
            return true;
        }
    }
    return false;
}

std::string Character::getName() const {
    return name;
}

const std::vector<Monster>& Character::getMonsters() const {
    return monsters;
}