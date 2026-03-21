#include "Monster.h"

Monster::Monster(const std::string& name, int health, int attackPower)
    : name(name), health(health), attackPower(attackPower) {}

std::string Monster::getName() const {
    return name;
}

int Monster::getHealth() const {
    return health;
}

int Monster::getAttackPower() const {
    return attackPower;
}

void Monster::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Monster::isAlive() const {
    return health > 0;
}

void Monster::attack(Monster& target) {
    if (isAlive()) {
        target.takeDamage(attackPower);
    }
}





