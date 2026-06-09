#ifndef CHARACTER_H
#define CHARACTER_H
#include "Monster.h"
#include "Item.h" // Add this include
#include <vector>

class Character {
public:
    Character(const std::string& name);
    
    void addMonster(const Monster& newMonster);
    void replaceMonster(int index, const Monster& m);
    bool hasAliveMonsters() const;
    std::string getName() const;
    const std::vector<Monster>& getMonsters() const;

    // New methods for Player Inventory
    void addItem(const Item& item);
    void removeItem(int index);
    std::vector<Item>& getItems(); // Notice: not const, so we can modify the bag

private:
    std::string name;
    std::vector<Monster> monsters; 
    std::vector<Item> items; // The player's inventory bag!
};

#endif