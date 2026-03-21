#ifndef CHARACTER_H
#define CHARACTER_H
#include "Monster.h"
#include <vector>

class Character {
public:
    Character(const std::string& name);
    
    void addMonster(const Monster& newMonster);
    
    void replaceMonster(int index, const Monster& m);

    bool hasAliveMonsters() const;

    std::string getName() const;

    const std::vector<Monster>& getMonsters() const;

private:
    std::string name;
    std::vector<Monster> monsters; // Vector to hold the character's monsters (max 4 monsters)
};

#endif // CHARACTER_H