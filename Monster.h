#ifndef MONSTER_H
#define MONSTER_H
#include <string>
class Monster {
public:

    Monster(const std::string& name, int health, int attackPower);
  
    void attack(Monster& target);
    
    void takeDamage(int damage);
   
    bool isAlive() const;
    
    std::string getName() const;
   
    int getHealth() const;

    int getAttackPower() const;

private:
    std::string name;
    int health;
    int attackPower;


};


#endif // Monster.h