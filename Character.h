#ifndef CHARACTER_H
#define CHARACTER_H
#include "Monster.h"
#include "Item.h"
#include <vector>

class Character {
public:
    Character(const std::string& name);
    
    void addMonster(const Monster& newMonster);
    void replaceMonster(int index, const Monster& m);
    bool hasAliveMonsters() const;
    std::string getName() const;
    const std::vector<Monster>& getMonsters() const;

    // Player Inventory Methods
    void addItem(const Item& item);
    void removeItem(int index);
    std::vector<Item>& getItems(); 

    int getTeamLevel() const;

private:
    std::string name;
    std::vector<Monster> monsters; 
    std::vector<Item> items; 
};

#endif