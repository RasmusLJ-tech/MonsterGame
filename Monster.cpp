#include "Monster.h"
#include <vector>
#include <iostream>

Monster::Monster(const std::string& name, int health, int attackPower)
    : name(name), health(health), attackPower(attackPower) {}

std::string Monster::getName() const {
    return name;
}

int Monster::getHealth() const {
    return health;
}

//Iteration 2 - expanded getAttackPower method to account for status effects
int Monster::getAttackPower() const {
    int currentAttack = attackPower;
    for (const auto& status : activeStatuses) {
        if (status.getType() == StatusType::STRENGTH_BUFF) {
            currentAttack += 2; // Example of a buff that increases attack power
        }
        if (status.getType() == StatusType::FROZEN) {
            currentAttack -= 2; // Example of a debuff that reduces attack power
        }
    }
    return (currentAttack < 0) ? 0 : currentAttack;
}

void Monster::takeDamage(int damage) { //Iteration 2 - added defense buff logic to takeDamage method
    // Check for defense buff before applying damage
    bool hasDefenseBuff = false;
    for (const auto& status : activeStatuses) {
        if (status.getType() == StatusType::DEFENSE_BUFF) {
            hasDefenseBuff = true;
            break;
        }
    }
    // If the monster has a defense buff, there's a 30% chance to block the entire attack
    if (hasDefenseBuff && (rand() % 100 < 30)) {
        std::cout << name << " BLOCKED the entire attack with its defense aura!" << std::endl;
        damage = 0; 
    }
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

//Iteration 2 - added addItem and addStatus methods to allow monsters to gain items and status effects during the game, as well as getter methods to access these lists
void Monster::addItem(const Item& item) {
    items.push_back(item);
}

void Monster::addStatus(const Status& status) {
    activeStatuses.push_back(status);
}

const std::vector<Item>& Monster::getItems() const {
    return items;
}

const std::vector<Status>& Monster::getActiveStatuses() const {
    return activeStatuses;
}

//Iteration 2 - added processStatuses method to handle status effects each turn
bool Monster::processStatuses() {
    bool canAct = true; // Assume monster can act unless a status prevents it
    auto it = activeStatuses.begin();

    while (it != activeStatuses.end()) {
        // Debuffs
        if (it->getType() == StatusType::POISON) {
        std::cout << name << " suffers 2 damage from poison!" << std::endl;
        takeDamage(2);
        }
        else if (it->getType() == StatusType::FROZEN) {
        std::cout << name << " is frozen solid and cannot move!" << std::endl;
        }
        else if (it->getType() == StatusType::CONFUSED) {    
        std::cout << name << " is confused and might hurt itself!" << std::endl;
        if (rand() % 2 == 0) { // 50% chance to hurt itself
            std::cout << name << " hurts itself in confusion!" << std::endl;
            takeDamage(1);
            canAct = false; // Confused prevents action if it hurts itself
        } else {
            std::cout << name << " manages to act despite the confusion!" << std::endl;
        }
        }
        else if (it->getType() == StatusType::BLEEDING) {   
        std::cout << name << " is bleeding and takes 1 damage!" << std::endl;
        takeDamage(1);
        }   

        // Buffs
        if (it->getType() == StatusType::STRENGTH_BUFF) {
        std::cout << name << "'s muscles are glowing with strength!" << std::endl;
        }
        else if (it->getType() == StatusType::DEFENSE_BUFF) {
        std::cout << name << " is covered by a defensive aura!" << std::endl;
        }
        else if (it->getType() == StatusType::SPEED_BUFF) {
        std::cout << name << " is moving lightning fast!" << std::endl;
        }

        // Reduce time remaining for the status effect
        it->reduceDuration();

        // Remove status if expired 
        if (it->getDuration() <= 0) {
            std::cout << name << "'s " << it->getStatusName() << " effect wore off." << std::endl;
            it = activeStatuses.erase(it);
        } else {
            ++it;
        }
    }
    return canAct;
}



