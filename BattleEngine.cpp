#include "BattleEngine.h"
#include <iostream>
#include <stdlib.h>

bool BattleEngine::startBattle(Character& player, Monster& enemy) {
    int playerTurn = (rand() % 2) + 1;

    // Battle loop - keeps running until either the player has no alive monsters or the enemy is defeated
    while (player.hasAliveMonsters() && enemy.isAlive()) {
        
        // Find the first alive monster of the player to be the active one for this turn
        Monster* activePlayerMonster = nullptr;
        for (size_t i = 0; i < player.getMonsters().size(); ++i) {
            if (player.getMonsters()[i].isAlive()) {
                activePlayerMonster = &const_cast<Monster&>(player.getMonsters()[i]);
                break;
            }
        }

        if (playerTurn == 1) { // Player's turn
            std::cout << "\n--- " << activePlayerMonster->getName() << "'s Turn ---" << std::endl;
            
            // 1. Status effects are processed at the start of the turn, which may prevent action or cause damage
            bool canAct = activePlayerMonster->processStatuses();
            if (!canAct) {
                std::cout << activePlayerMonster->getName() << " cannot move this turn!" << std::endl;
                playerTurn = 2;
                continue;
            }

            // 2. Battle menu (Attack or Item)
            int actionChoice;
            std::cout << "1. Attack\n2. Use Item\nChoose action: ";
            std::cin >> actionChoice;

            if (actionChoice == 1) {
                // Original attack logic
                activePlayerMonster->attack(enemy);
                
                // BLEEDING Logic: If the player's monster has the BLEEDING status, it takes extra damage after attacking
                for (const auto& s : activePlayerMonster->getActiveStatuses()) {
                    if (s.getType() == StatusType::BLEEDING) {
                        std::cout << activePlayerMonster->getName() << " bleeds heavily from attacking and takes 2 extra damage!" << std::endl;
                        activePlayerMonster->takeDamage(2);
                    }
                }
            } 
            else if (actionChoice == 2) {
                // Use Item Logic: Player can choose an item from their active monster's inventory to use against the enemy
                if (activePlayerMonster->getItems().empty()) {
                    std::cout << "This monster has no items! You waste your turn looking in your empty bag." << std::endl;
                } else {
                    std::cout << "Select an item to use:" << std::endl;
                    for (size_t i = 0; i < activePlayerMonster->getItems().size(); ++i) {
                        std::cout << i + 1 << ". " << activePlayerMonster->getItems()[i].getName() << std::endl;
                    }
                    int itemChoice;
                    std::cin >> itemChoice;
                    
                    if (itemChoice >= 1 && itemChoice <= (int)activePlayerMonster->getItems().size()) {
                        Item chosenItem = activePlayerMonster->getItems()[itemChoice - 1];
                        std::cout << activePlayerMonster->getName() << " used " << chosenItem.getName() << "!" << std::endl;
                        
                        // Apply item damage to the enemy
                        enemy.takeDamage(chosenItem.getDamage());
                        
                        // Roll for status chance
                        if (chosenItem.getStatusEffect() != StatusType::NONE) {
                            if ((rand() % 100) < chosenItem.getChance()) {
                                Status newStatus(chosenItem.getStatusEffect(), chosenItem.getStatusDuration());
                                enemy.addStatus(newStatus);
                                std::cout << enemy.getName() << " was inflicted with " << newStatus.getStatusName() << "!" << std::endl;
                            }
                        }
                    }
                }
            }

            // 3. SPEED_BUFF Logic: If the player's monster has a SPEED_BUFF status, it has a chance to get an extra turn immediately after attacking
            bool extraTurn = false;
            for (const auto& s : activePlayerMonster->getActiveStatuses()) {
                if (s.getType() == StatusType::SPEED_BUFF) {
                    if (rand() % 100 < 30) { // 30% chance for extra attack
                        std::cout << activePlayerMonster->getName() << " utilizes lightning speed to get an extra strike!" << std::endl;
                        extraTurn = true;
                    }
                }
            }

            if (!extraTurn) {
                playerTurn = 2; // Shift to enemy's turn if no extra turn is granted
            }

        } else { // Enemy's turn
            std::cout << "\n--- " << enemy.getName() << "'s Turn ---" << std::endl;
            
            bool canAct = enemy.processStatuses();
            if (!canAct) {
                playerTurn = 1;
                continue;
            }

            // Enemy attacks randomly one of the player's alive monsters
            enemy.attack(*activePlayerMonster);
            playerTurn = 1;
        }
    }

    // Return true if the player wins (enemy is defeated), false if the player loses (all player's monsters are defeated)
    return enemy.getHealth() <= 0;
}