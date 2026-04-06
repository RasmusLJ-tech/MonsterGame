#ifndef ITEM_H
#define ITEM_H
#include <string>
#include "Status.h"

class Item {
public:
    Item(std::string name, int damage, StatusType sType, int sDuration, int chance); 
    
    std::string getName() const;
    int getDamage() const;
    StatusType getStatusEffect() const;
    int getStatusDuration() const;
    int getChance() const;

private:
    std::string name;
    int damage;
    StatusType statusEffect;
    int statusDuration;
    int applyChance; // 0-100%
};

#endif