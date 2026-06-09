#ifndef MONSTER_H
#define MONSTER_H
#include <string>
#include <vector>
#include "Item.h"
#include "Status.h"

class Monster {
public:

    Monster(const std::string& name, int health, int attackPower);
  
    void attack(Monster& target);
    
    void takeDamage(int damage);
   
    bool isAlive() const;
    
    std::string getName() const;
   
    int getHealth() const;

    int getAttackPower() const;

    void addItem(const Item& item);
    
    void addStatus(const Status& status);

    bool processStatuses(); 

    const std::vector<Item>& getItems() const;

    const std::vector<Status>& getActiveStatuses() const;

private:
    std::string name;
    int health;
    int attackPower;
    std::vector<Item> items;
    std::vector<Status> activeStatuses;

};

#endif 