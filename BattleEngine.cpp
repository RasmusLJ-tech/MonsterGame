#include "BattleEngine.h"
#include <iostream>
#include <stdlib.h>

bool BattleEngine::startBattle(Character& player, Monster& enemy) {
    // FIX: Force player to always start first to ensure fair gameplay
    int playerTurn = 1; 

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

        // Display current HP status for both active monsters at the start of every round
        std::cout << "\n================= BATTLE STATUS =================" << std::endl;
        std::cout << " Active Player Monster: " << activePlayerMonster->getName() 
                  << " (" << activePlayerMonster->getHealth() << " HP)" << std::endl;
        std::cout << " Active Enemy Monster:  " << enemy.getName() 
                  << " (" << enemy.getHealth() << " HP)" << std::endl;
        std::cout << "=================================================" << std::endl;

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
            } else if (actionChoice == 2) {
                //  Look inside the PLAYER'S items 
                if (player.getItems().empty()) {
                    std::cout << "Your inventory is empty! You waste your turn rummaging through your bag." << std::endl;
                } else {
                    std::cout << "\n--- Your Inventory ---" << std::endl;
                    for (size_t i = 0; i < player.getItems().size(); ++i) {
                        std::cout << i + 1 << ". " << player.getItems()[i].getName() << std::endl;
                    }
                    std::cout << "Select an item to use (or 0 to go back): ";
                    int itemChoice;
                    std::cin >> itemChoice;
                    
                    if (itemChoice >= 1 && itemChoice <= (int)player.getItems().size()) {
                        Item chosenItem = player.getItems()[itemChoice - 1];
                        
                        // TARGET SELECTION MENU: Decide who to use it on
                        std::cout << "\nWho do you want to use [" << chosenItem.getName() << "] on?" << std::endl;
                        std::cout << "1. Your active monster (" << activePlayerMonster->getName() << ")" << std::endl;
                        std::cout << "2. The enemy monster (" << enemy.getName() << ")" << std::endl;
                        std::cout << "Choose target: ";
                        int targetChoice;
                        std::cin >> targetChoice;

                        std::cout << "\nYou used [" << chosenItem.getName() << "]!" << std::endl;

                        if (targetChoice == 1) { 
                            // Target: YOUR OWN MONSTER (Healing / Buffs)
                            // Negative damage means it subtracts a negative number (healing!)
                            activePlayerMonster->takeDamage(chosenItem.getDamage());
                            
                            if (chosenItem.getStatusEffect() != StatusType::NONE) {
                                if ((rand() % 100) < chosenItem.getChance()) {
                                    Status newStatus(chosenItem.getStatusEffect(), chosenItem.getStatusDuration());
                                    activePlayerMonster->addStatus(newStatus);
                                    std::cout << activePlayerMonster->getName() << " gained " << newStatus.getStatusName() << "!" << std::endl;
                                }
                            }
                        } 
                        else { 
                            // Target: THE ENEMY (Damage / Debuffs)
                            enemy.takeDamage(chosenItem.getDamage());
                            
                            if (chosenItem.getStatusEffect() != StatusType::NONE) {
                                if ((rand() % 100) < chosenItem.getChance()) {
                                    Status newStatus(chosenItem.getStatusEffect(), chosenItem.getStatusDuration());
                                    enemy.addStatus(newStatus);
                                    std::cout << enemy.getName() << " was inflicted with " << newStatus.getStatusName() << "!" << std::endl;
                                }
                            }
                        }

                        // Remove the item from the player's shared bag after successful use
                        player.removeItem(itemChoice - 1);
                        playerTurn = 2; // End player's turn
                    }
                }
            }

            // 3. SPEED_BUFF Logic: If the player's monster has a SPEED_BUFF status, it has a chance to get an extra turn immediately after attacking
            bool extraTurn = false;
            for (const auto& s : activePlayerMonster->getActiveStatuses()) {
                if (s.getType() == StatusType::SPEED_BUFF) {
                    if (rand() % 100 < 50) { // 50% chance for extra attack
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