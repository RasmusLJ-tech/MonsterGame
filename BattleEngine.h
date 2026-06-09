#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "Character.h"
#include "Monster.h"
#include "Dungeon.h" 

class BattleEngine {
public:
    // Returns true if the player wins, false if they lose
    static bool startDungeonBattle(Character& player, Dungeon& dungeon);
};

#endif