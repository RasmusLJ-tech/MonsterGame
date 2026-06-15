#include <iostream>
#include <vector>
#include <string>
#include "Character.h"
#include "Monster.h"
#include <stdlib.h> // For rand() and srand()
#include <iomanip>
#include "BattleEngine.h"
#include "Dungeon.h"
#include "Database.h"

int main(){
    int choiceMenu, choiceInsideGame, choiceBattle, playerTurn, choiceKeepMonster, replaceIndex;
    std::string nameInput;
    
    // Initialize Database
    Database db("spildata.db");
    db.initializeDatabase();

    Character* player = nullptr;
    
    // Iteration 2
    // Create a pool of concrete items tied to your status effects
    Item healthPotion("Health Potion", -15, StatusType::NONE, 0, 0);       
    Item poisonDagger("Poison Dagger", 2, StatusType::POISON, 3, 100);     
    Item iceBomb("Ice Bomb", 1, StatusType::FROZEN, 1, 100);               
    Item smokeBomb("Smoke Bomb", 0, StatusType::CONFUSED, 3, 80);          
    Item jaggedSpear("Jagged Spear", 4, StatusType::BLEEDING, 3, 100);     
    Item rageElixir("Rage Elixir", 0, StatusType::STRENGTH_BUFF, 3, 100);  
    Item ironShield("Iron Shield", 0, StatusType::DEFENSE_BUFF, 5, 100);   
    Item adrenalineShot("Adrenaline Shot", 0, StatusType::SPEED_BUFF, 4, 100);   

    // Enemy monsters Iteration 1
    std::vector<Monster> enemyMonsters;
    enemyMonsters.push_back(Monster("Weak goblin", 4, 2));
    enemyMonsters.push_back(Monster("Strong goblin", 8, 3));
    // ... remaining monsters ...

    // MAIN GAME LOOP
    while(true) {
        std::cout << "\n=== MAIN MENU ===" << std::endl;
        std::cout << "1. Start / Continue Game" << std::endl;
        std::cout << "2. Create New Character" << std::endl;
        std::cout << "3. Load Existing Character" << std::endl;
        std::cout << "4. View Game Statistics" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choiceMenu;

        if (choiceMenu == 1) {
            std::cout << "Starting game..." << std::endl;
            if (player == nullptr) {
                std::cout << "Please create or load a character before starting the game." << std::endl;
            } 
            else {
                // INSIDE GAME LOOP
                while (player != nullptr && player->hasAliveMonsters()) {
                    std::cout << "\n--- CAMP MENU ---" << std::endl;
                    std::cout << "1. Enter Dungeon (Battle)" << std::endl;
                    std::cout << "2. Save Game" << std::endl;
                    std::cout << "3. Exit to Main Menu" << std::endl;
                    std::cout << "Choice: ";
                    std::cin >> choiceInsideGame;

                    if (choiceInsideGame == 1) {
                        // Display Active Roster stats
                        std::cout << "\n--- YOUR SQUAD ---" << std::endl;
                        for (const Monster& monster : player->getMonsters()) {
                            std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
                        }

                        // Display Player Inventory
                        std::cout << "--- YOUR INVENTORY ---" << std::endl;
                        if (player->getItems().empty()) {
                            std::cout << " [Empty Inventory Bag]" << std::endl;
                        } else {
                            for (size_t i = 0; i < player->getItems().size(); ++i) {
                                std::cout << " - " << player->getItems()[i].getName() << std::endl;
                            }
                        }

                        // Select customizable dungeon modifiers
                        std::cout << "\nSelect Dungeon Difficulty:" << std::endl;
                        std::cout << "1. Easy Dungeon (1 Monster - Scaled Down)" << std::endl;
                        std::cout << "2. Medium Dungeon (2 Fights in a row - Normal Scaling)" << std::endl;
                        std::cout << "3. Hard Dungeon (3 Fights in a row - High Scaling!)" << std::endl;
                        std::cout << "Choose difficulty setting (1-3): ";
                        int diffChoice;
                        std::cin >> diffChoice;

                        if (diffChoice < 1 || diffChoice > 3) {
                            std::cout << "Invalid setting selection!" << std::endl;
                            continue;
                        }

                        Dungeon dynamicDungeon("Mysterious Cavern", player->getTeamLevel(), diffChoice);
                        bool dungeonCleared = BattleEngine::startDungeonBattle(*player, dynamicDungeon);

                        if (dungeonCleared) {
                            std::cout << "\nVICTORY! You completely cleared " << dynamicDungeon.getName() << "!" << std::endl;
                            std::cout << "--- DUNGEON REWARD CHEST ---" << std::endl;

                            
                          
                                 size_t lootDropsCount = dynamicDungeon.getEnemies().size();

                                // STATISTICS UPDATE 
                                // 1. Update total kills based on the number of enemies in the dungeon
                                for (size_t i = 0; i < lootDropsCount; ++i) {
                                       db.incrementGlobalKills();
                                    }

                                // 2. Update "Most used monster" (Assuming you used the first monster in your squad)
                                if (!player->getMonsters().empty()) {
                                     db.incrementMonsterUsage(player->getName(), player->getMonsters()[0].getName());
                                    }
                               
                            for (size_t drop = 0; drop < lootDropsCount; ++drop) {
                                Item droppedItem = healthPotion; 
                                int lootRoll = rand() % 100;    

                                // Reward table logic (kept exactly as your original code)
                                if (diffChoice == 1) {
                                    if (lootRoll < 25) droppedItem = healthPotion;
                                    else if (lootRoll < 50) droppedItem = poisonDagger;
                                    else if (lootRoll < 75) droppedItem = iceBomb;
                                    else droppedItem = smokeBomb;
                                } 
                                else if (diffChoice == 2) {
                                    if (lootRoll < 20) droppedItem = healthPotion;
                                    else if (lootRoll < 40) droppedItem = poisonDagger;
                                    else if (lootRoll < 60) droppedItem = adrenalineShot;
                                    else if (lootRoll < 80) droppedItem = jaggedSpear;
                                    else droppedItem = ironShield; 
                                } 
                                else {
                                    if (lootRoll < 15) droppedItem = adrenalineShot;
                                    else if (lootRoll < 35) droppedItem = jaggedSpear;
                                    else if (lootRoll < 55) droppedItem = ironShield;
                                    else if (lootRoll < 80) droppedItem = rageElixir; 
                                    else droppedItem = healthPotion;
                                }

                                std::cout << "-> Found a [" << droppedItem.getName() << "]!" << std::endl;
                                player->addItem(droppedItem);
                            }
                            std::cout << "All items have been added to your shared inventory bag!" << std::endl;

                            // POST-BATTLE SQUAD RESTORATION
                            bool teamNeededHealing = false;
                            for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                if (player->getMonsters()[i].getHealth() < 4) {
                                    const_cast<Monster&>(player->getMonsters()[i]).reviveAndHeal(4);
                                    teamNeededHealing = true;
                                }
                            }

                            if (teamNeededHealing) {
                                std::cout << "\nYour injured party members rest. They have been successfully revived and healed!" << std::endl;
                            } else {
                                std::cout << "\nYour squad emerges flawless from the battle! No healing was required." << std::endl;
                            }

                            // CAPTURE MECHANICS
                            Monster lastDefeatedMonster = dynamicDungeon.getEnemies().back();
                            lastDefeatedMonster.reviveAndHeal(4);
                            std::cout << "\nYou can now choose to keep the defeated [" << lastDefeatedMonster.getName() << "]:" << std::endl;
                            std::cout << "1. Yes\n2. No\nChoice: ";
                            std::cin >> choiceKeepMonster;

                            if (choiceKeepMonster == 1) {
                                if (player->getMonsters().size() < 4) { 
                                    player->addMonster(lastDefeatedMonster);
                                    std::cout << "Successfully added " << lastDefeatedMonster.getName() << " to your team!" << std::endl;
                                } else { 
                                    std::cout << "Your squad is full (Max 4). Choose a monster to replace or cancel:" << std::endl;
                                    for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                        std::cout << i + 1 << ". " << player->getMonsters()[i].getName() 
                                                  << " (HP: " << player->getMonsters()[i].getHealth() 
                                                  << ", Attack: " << player->getMonsters()[i].getAttackPower() << ")" << std::endl;
                                    }
                                    std::cout << "5. Cancel (Do not keep the new monster)" << std::endl;
                                    std::cout << "Enter your choice (1-5): ";
                                    std::cin >> replaceIndex;
                                    
                                    if (replaceIndex >= 1 && replaceIndex <= 4) {
                                        player->replaceMonster(replaceIndex - 1, lastDefeatedMonster);
                                        std::cout << "Replaced old team member with " << lastDefeatedMonster.getName() << "!" << std::endl;
                                    } 
                                    else if (replaceIndex == 5) {
                                        std::cout << "Replacement cancelled." << std::endl;
                                    } 
                                    else {
                                        std::cout << "Invalid choice. You did not replace any monster." << std::endl;
                                    }
                                }
                            }

                        } else { 
                            std::cout << "\nYou limped out of the cavern ruins in utter defeat..." << std::endl;
                            break; 
                        }        
                    } 
                    else if (choiceInsideGame == 2) {
                        // SAVE GAME FEATURE
                        db.saveCharacter(*player);
                    }
                    else if (choiceInsideGame == 3) {
                        std::cout << "Exiting to Main Menu..." << std::endl;
                        break; 
                    } 
                    else {
                        std::cout << "Invalid choice. Please try again." << std::endl;
                    }
                } // End of inner while loop
            }
        } 
        else if (choiceMenu == 2) { 
            std::cout << "Creating a character..." << std::endl;
            std::cout << "Enter character name: ";
            std::cin >> nameInput;
            
            if (player != nullptr) {
                delete player;
            }
            player = new Character(nameInput);
            
            Monster monster1("Hest", 4, 1);
            Monster monster2("Hest", 4, 1);

            player->addItem(healthPotion);
            player->addItem(poisonDagger); 
            player->addItem(iceBomb); 
            
            player->addMonster(monster1);
            player->addMonster(monster2);

            std::cout << "Player: " << player->getName() << " has been created!" << std::endl;
            
            // Auto-save the new character to the database
            db.saveCharacter(*player);
        } 
        else if (choiceMenu == 3) {
            // LOAD CHARACTER FEATURE
            std::vector<std::string> savedPlayers = db.getAllPlayerNames();

            if (savedPlayers.empty()) {
                std::cout << "\n[Info] No saved characters found in the database!" << std::endl;
            } else {
                std::cout << "\n--- SAVED CHARACTERS ---" << std::endl;
                for (size_t i = 0; i < savedPlayers.size(); ++i) {
                    std::cout << i + 1 << ". " << savedPlayers[i] << std::endl;
                }
                std::cout << "------------------------" << std::endl;
                std::cout << "Select a character to load (number): ";
                
                int pickNum;
                std::cin >> pickNum;

                if (pickNum > 0 && pickNum <= static_cast<int>(savedPlayers.size())) {
                    if (player != nullptr) {
                        delete player; // Clean up current before loading
                    }
                    std::string chosenName = savedPlayers[pickNum - 1];
                    player = db.loadCharacter(chosenName);

                    // --- NEW FEATURE: Print loaded squad and inventory ---
                    if (player != nullptr) {
                        std::cout << "\n--- LOADED SQUAD ---" << std::endl;
                        for (const Monster& monster : player->getMonsters()) {
                            std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() 
                                      << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
                        }

                        std::cout << "--- LOADED INVENTORY ---" << std::endl;
                        if (player->getItems().empty()) {
                            std::cout << " [Empty Inventory Bag]" << std::endl;
                        } else {
                            for (size_t i = 0; i < player->getItems().size(); ++i) {
                                std::cout << " - " << player->getItems()[i].getName() << std::endl;
                            }
                        }
                        std::cout << "------------------------" << std::endl;
                    }
                    // -----------------------------------------------------

                } else {
                    std::cout << "\n[Error] Invalid selection." << std::endl;
                }
            }
        }
        else if (choiceMenu == 4) {
            // STATISTICS FEATURE
            db.printGameStatistics();
        }
        else if (choiceMenu == 5) { 
            std::cout << "Exiting the game. Goodbye!" << std::endl;
            if (player != nullptr) {
                delete player; 
            }
            return 0; 
        } 
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } 
    
    return 0;
}