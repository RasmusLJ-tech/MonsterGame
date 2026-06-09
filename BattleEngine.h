#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "Character.h"
#include "Monster.h"

class BattleEngine {
public:
    // Returnerer true hvis spilleren vinder kampen, false hvis de taber
    static bool startBattle(Character& player, Monster& enemy);
};

#endif