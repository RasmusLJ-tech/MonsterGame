#include <iostream>
#include "Character.h"
#include "Monster.h"
#include <stdlib.h> // For rand() and srand()
#include <iomanip>
#include "BattleEngine.h"
#include "Dungeon.h"

int main(){
    int choiceMenu, choiceInsideGame, choiceBattle, playerTurn, choiceKeepMonster, replaceIndex;
    std::string nameInput;
    
    Character* player = nullptr;
    
    // Create a pool of concrete items tied to your status effects
    // Format: Item(name, damage, statusType, duration, chance)
    Item healthPotion("Health Potion", -15, StatusType::NONE, 0, 0);       // Heals your active monster
    Item poisonDagger("Poison Dagger", 2, StatusType::POISON, 3, 100);     // Inflicts Poison (2 damage/turn)
    Item iceBomb("Ice Bomb", 1, StatusType::FROZEN, 1, 100);               // Inflicts Frozen (Stun/Skip turn)
    Item smokeBomb("Smoke Bomb", 0, StatusType::CONFUSED, 3, 80);          // 80% chance to Conflict Confused
    Item jaggedSpear("Jagged Spear", 4, StatusType::BLEEDING, 3, 100);     // Inflicts Bleeding (1 damage/turn + extra on attack)
    Item rageElixir("Rage Elixir", 0, StatusType::STRENGTH_BUFF, 3, 100);  // Increases attack power by 2
    Item ironShield("Iron Shield", 0, StatusType::DEFENSE_BUFF, 5, 100);   // 30% chance to block attacks
    Item adrenalineShot("Adrenaline Shot", 0, StatusType::SPEED_BUFF, 4, 100);   // 55% chance for an extra turn

    // Enemy monsters Iteration 1
    std::vector<Monster> enemyMonsters;
    enemyMonsters.push_back(Monster("Weak goblin", 4, 2));
    enemyMonsters.push_back(Monster("Strong goblin", 8, 3));
    enemyMonsters.push_back(Monster("Giant goblin", 10, 4));
    enemyMonsters.push_back(Monster("Boss goblin", 15, 5));
    enemyMonsters.push_back(Monster("Monkey king", 30, 5));
    enemyMonsters.push_back(Monster("Unicorn", 50, 8));
    enemyMonsters.push_back(Monster("Dragon", 100, 10));

    // MAIN GAME LOOP
    while(true) {
        std::cout << "\nWelcome to the Monster Game!" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. Make character" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choiceMenu;

        if (choiceMenu == 1) {
            std::cout << "Starting game..." << std::endl;
            if (player == nullptr) {
                std::cout << "Please create or pick a character before starting the game." << std::endl;
            } 
            else {
                // INSIDE GAME LOOP (Added safety check for nullptr)
                while (player != nullptr && player->hasAliveMonsters()) {
                    std::cout << "\nGame is running..." << std::endl;
                    std::cout << "1. Battle" << std::endl;
                    std::cout << "2. Exit to Main Menu" << std::endl;
                    std::cout << "Choice: ";
                    std::cin >> choiceInsideGame;

                    if (choiceInsideGame == 1) {
                        // Display Active Roster stats before combat selection
                        std::cout << "\n--- YOUR SQUAD ---" << std::endl;
                        for (const Monster& monster : player->getMonsters()) {
                            std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
                        }

                        // Display Player Inventory contents before selecting difficulty
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

                        // Instantiate a scalable Dungeon dynamically tracking the player's team level metrics
                        Dungeon dynamicDungeon("Mysterious Cavern", player->getTeamLevel(), diffChoice);

                        // Execute the back-to-back gauntlet interface
                        bool dungeonCleared = BattleEngine::startDungeonBattle(*player, dynamicDungeon);

                        if (dungeonCleared) {
                            std::cout << "\nVICTORY! You completely cleared " << dynamicDungeon.getName() << "!" << std::endl;
                            std::cout << "--- DUNGEON REWARD CHEST ---" << std::endl;

                            // Scale the number of drops directly by the amount of enemies defeated
                            size_t lootDropsCount = dynamicDungeon.getEnemies().size();

                            for (size_t drop = 0; drop < lootDropsCount; ++drop) {
                                Item droppedItem = healthPotion; // Default baseline fallback item
                                int lootRoll = rand() % 100;    // 0 to 99 roll for percentage tiers

                                // Reward table tailored dynamically to the selected difficulty tier
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
                                player->addItem(droppedItem); // Transferred securely into character bag
                            }
                            std::cout << "All items have been added to your shared inventory bag!" << std::endl;

                            // POST-BATTLE SQUAD RESTORATION (Only revives fainted or injured monsters)
                            bool teamNeededHealing = false;
                            for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                // Check if the monster is dead (0 HP) or has taken damage (less than max 4 HP)
                                if (player->getMonsters()[i].getHealth() < 4) {
                                    const_cast<Monster&>(player->getMonsters()[i]).takeDamage(-100); // Heals back to full
                                    teamNeededHealing = true;
                                }
                            }

                            if (teamNeededHealing) {
                                std::cout << "\nYour injured party members rest. They have been successfully revived and healed!" << std::endl;
                            } else {
                                std::cout << "\nYour squad emerges flawless from the battle! No healing was required." << std::endl;
                            }

                            // CAPTURE MECHANICS: Allow player to keep the last defeated dungeon boss/monster
                            Monster lastDefeatedMonster = dynamicDungeon.getEnemies().back();
                            std::cout << "\nYou can now choose to keep the defeated [" << lastDefeatedMonster.getName() << "]:" << std::endl;
                            std::cout << "1. Yes\n2. No\nChoice: ";
                            std::cin >> choiceKeepMonster;

                            if (choiceKeepMonster == 1) {
                                if (player->getMonsters().size() < 4) { 
                                    // Room available, add directly
                                    player->addMonster(lastDefeatedMonster);
                                    std::cout << "Successfully added " << lastDefeatedMonster.getName() << " to your team!" << std::endl;
                                } else { 
                                    // Team full, must choose a replacement target index or cancel
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
                                        std::cout << "Replacement cancelled. You decided not to keep " << lastDefeatedMonster.getName() << "." << std::endl;
                                    } 
                                    else {
                                        std::cout << "Invalid choice. You did not replace any monster." << std::endl;
                                    }
                                }
                            }

                        } else { 
                            std::cout << "\nYou limped out of the cavern ruins in utter defeat..." << std::endl;
                            break; // Exits the inner loop since the player lost all monsters
                        }        
                    } 
                    else if (choiceInsideGame == 2) {
                        std::cout << "Exiting to Main Menu..." << std::endl;
                        delete player;
                        player = nullptr;
                        break; // Safely breaks out of the 'while(player != nullptr)' loop
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
            
            // Clean up existing player if making a new one to prevent memory leaks
            if (player != nullptr) {
                delete player;
            }
            player = new Character(nameInput);
            
            Monster monster1("Hest", 4, 1);
            Monster monster2("Hest", 4, 1);

            // Give the player a starting kit in their inventory from the start
            player->addItem(healthPotion);
            player->addItem(poisonDagger); 
            player->addItem(iceBomb); 
            
            player->addMonster(monster1);
            player->addMonster(monster2);

            std::cout << "Player: " << player->getName() << " has the following monsters:" << std::endl;
            for (const Monster& monster : player->getMonsters()) {
                std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
            }
        } 
        else if (choiceMenu == 3) { 
            std::cout << "Exiting the game. Goodbye!" << std::endl;
            if (player != nullptr) {
                delete player; 
            }
            return 0; 
        } 
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } // End of outer while(true) loop
    
    return 0;
}