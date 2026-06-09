#include "Character.h"
#include "Monster.h"

Character::Character(const std::string& name) : name(name) {}

void Character::addMonster(const Monster& newMonster) {
    if (monsters.size() < 4) {
        monsters.push_back(newMonster);
    }
}

void Character::replaceMonster(int index, const Monster& m) {
    if (index >= 0 && index < monsters.size()) {
        monsters[index] = m;
    }
}

bool Character::hasAliveMonsters() const {
    for (const Monster& monster : monsters) {
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

// Nye inventory funktioner
void Character::addItem(const Item& item) {
    items.push_back(item);
}

void Character::removeItem(int index) {
    if (index >= 0 && index < items.size()) {
        items.erase(items.begin() + index);
    }
}

std::vector<Item>& Character::getItems() {
    return items;
}

int Character::getTeamLevel() const {
    int totalStrength = 0;
    if (monsters.empty()) return 1;
    
    for (const auto& monster : monsters) {
        totalStrength += (monster.getHealth() + monster.getAttackPower());
    }
    // Returnerer et gennemsnit af dine monstres rå stats
    return totalStrength / monsters.size();
}