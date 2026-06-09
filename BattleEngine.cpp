#include "BattleEngine.h"
#include <iostream>
#include <stdlib.h>

// Executes consecutive monster battles inside a single dungeon run
bool BattleEngine::startDungeonBattle(Character& player, Dungeon& dungeon) {
    std::cout << "\n=================================================" << std::endl;
    std::cout << " ENTERING DUNGEON: " << dungeon.getName() << std::endl;
    std::cout << " Prepare to face " << dungeon.getEnemies().size() << " monsters in a row!" << std::endl;
    std::cout << "=================================================" << std::endl;

    // Loop through all generated enemies sequentially
    for (size_t monsterIndex = 0; monsterIndex < dungeon.getEnemies().size(); ++monsterIndex) {
        Monster& enemy = dungeon.getEnemies()[monsterIndex];
        
        std::cout << "\n>>> A wild " << enemy.getName() << " blocks your path! <<<" << std::endl;
        int playerTurn = 1; // Player always acts first for fair initialization

        // Active turn loop against the current single enemy target
        while (player.hasAliveMonsters() && enemy.isAlive()) {
            
            // Fetch the player's first available alive monster
            Monster* activePlayerMonster = nullptr;
            for (size_t i = 0; i < player.getMonsters().size(); ++i) {
                if (player.getMonsters()[i].isAlive()) {
                    activePlayerMonster = &const_cast<Monster&>(player.getMonsters()[i]);
                    break;
                }
            }

            // Display current HP metrics for active combatants
            std::cout << "\n================= BATTLE STATUS =================" << std::endl;
            std::cout << " Active Player Monster: " << activePlayerMonster->getName() 
                      << " (" << activePlayerMonster->getHealth() << " HP)" << std::endl;
            std::cout << " Active Enemy Monster:  " << enemy.getName() 
                      << " (" << enemy.getHealth() << " HP)" << std::endl;
            std::cout << "=================================================" << std::endl;

            if (playerTurn == 1) { // Player Action Phase
                std::cout << "\n--- " << activePlayerMonster->getName() << "'s Turn ---" << std::endl;
                
                // Process ticks for active status effects before allowing choices
                bool canAct = activePlayerMonster->processStatuses();
                if (!canAct) {
                    std::cout << activePlayerMonster->getName() << " cannot move this turn!" << std::endl;
                    playerTurn = 2; 
                    continue;
                }

                int actionChoice;
                std::cout << "1. Attack\n2. Use Item\nChoose action: ";
                std::cin >> actionChoice;

                if (actionChoice == 1) {
                    activePlayerMonster->attack(enemy);
                    
                    // Bleeding reaction check
                    for (const auto& s : activePlayerMonster->getActiveStatuses()) {
                        if (s.getType() == StatusType::BLEEDING) {
                            std::cout << activePlayerMonster->getName() << " bleeds heavily and takes 2 extra damage!" << std::endl;
                            activePlayerMonster->takeDamage(2);
                        }
                    }
                    playerTurn = 2; 
                } 
                else if (actionChoice == 2) {
                    // Check the global shared PLAYER inventory bag
                    if (player.getItems().empty()) {
                        std::cout << "Your inventory is empty! You waste your turn rummaging through your bag." << std::endl;
                        playerTurn = 2;
                    } else {
                        std::cout << "\n--- Your Inventory ---" << std::endl;
                        for (size_t i = 0; i < player.getItems().size(); ++i) {
                            std::cout << i + 1 << ". " << player.getItems()[i].getName() << std::endl;
                        }
                        std::cout << "Select an item to use: ";
                        int itemChoice;
                        std::cin >> itemChoice;
                        
                        if (itemChoice >= 1 && itemChoice <= (int)player.getItems().size()) {
                            Item chosenItem = player.getItems()[itemChoice - 1];
                            
                            // Dynamic target allocation interface
                            std::cout << "Use [" << chosenItem.getName() << "] on:\n1. Your Active Monster\n2. The Enemy Target\nChoice: ";
                            int targetChoice;
                            std::cin >> targetChoice;

                            if (targetChoice == 1) { // Apply attributes to your squad
                                activePlayerMonster->takeDamage(chosenItem.getDamage());
                                if (chosenItem.getStatusEffect() != StatusType::NONE && (rand() % 100) < chosenItem.getChance()) {
                                    activePlayerMonster->addStatus(Status(chosenItem.getStatusEffect(), chosenItem.getStatusDuration()));
                                }
                            } else { // Cast attributes onto enemy target
                                enemy.takeDamage(chosenItem.getDamage());
                                if (chosenItem.getStatusEffect() != StatusType::NONE && (rand() % 100) < chosenItem.getChance()) {
                                    enemy.addStatus(Status(chosenItem.getStatusEffect(), chosenItem.getStatusDuration()));
                                }
                            }
                            // Consume the item from player inventory array
                            player.removeItem(itemChoice - 1);
                            playerTurn = 2;
                        }
                    }
                }

                // Speed buff extra round validator
                for (const auto& s : activePlayerMonster->getActiveStatuses()) {
                    if (s.getType() == StatusType::SPEED_BUFF && rand() % 100 < 30) {
                        std::cout << activePlayerMonster->getName() << " utilizes lightning speed to secure an extra strike!" << std::endl;
                        playerTurn = 1; 
                    }
                }

            } else { // Enemy Action Phase
                std::cout << "\n--- " << enemy.getName() << "'s Turn ---" << std::endl;
                if (!enemy.processStatuses()) {
                    playerTurn = 1;
                    continue;
                }
                enemy.attack(*activePlayerMonster);
                playerTurn = 1;
            }
        }

        // Return failure immediately if the entire party goes down inside the dungeon
        if (!player.hasAliveMonsters()) {
            std::cout << "\nAll your monsters fainted! You failed to clear the dungeon..." << std::endl;
            return false;
        }

        std::cout << "\nYou defeated " << enemy.getName() << "!" << std::endl;
        if (monsterIndex < dungeon.getEnemies().size() - 1) {
            std::cout << "Keep guard! The next dungeon guardian is stepping forward..." << std::endl;
        }
    }

    // Surviving all rounds triggers a true state for clean sweep clearance
    return true;
}