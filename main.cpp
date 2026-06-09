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
    Item smokeBomb("Smoke Bomb", 0, StatusType::CONFUSED, 2, 80);          // 80% chance to Conflict Confused
    Item jaggedSpear("Jagged Spear", 4, StatusType::BLEEDING, 3, 100);     // Inflicts Bleeding (1 damage/turn + extra on attack)
    Item rageElixir("Rage Elixir", 0, StatusType::STRENGTH_BUFF, 3, 100);  // Increases attack power by 2
    Item ironShield("Iron Shield", 0, StatusType::DEFENSE_BUFF, 3, 100);   // 30% chance to block attacks
    Item adrenalineShot("Adrenaline Shot", 0, StatusType::SPEED_BUFF, 2, 100);   // 30% chance for an extra turn

    // Enemy monsters
    std::vector<Monster> enemyMonsters;
    Monster enemy1("Weak goblin", 4, 2);
    Monster enemy2("Strong goblin", 8, 3);
    Monster enemy3("Giant goblin", 10, 4);
    Monster enemy4("Boss goblin", 15, 5);
    Monster enemy5("Monkey king", 30, 5);
    Monster enemy6("Unicorn", 50, 8);
    Monster enemy7("Dragon", 100, 10);
    
    enemyMonsters.push_back(enemy1);
    enemyMonsters.push_back(enemy2);
    enemyMonsters.push_back(enemy3);
    enemyMonsters.push_back(enemy4);
    enemyMonsters.push_back(enemy5);
    enemyMonsters.push_back(enemy6);
    enemyMonsters.push_back(enemy7);

    while(true) {
        std::cout << "Welcome to the Monster Game!" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. Make character" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cin >> choiceMenu;

        if (choiceMenu == 1) {
            std::cout << "Starting game..." << std::endl;
            if (player == nullptr) {
                std::cout << "Please create a character before starting the game." << std::endl;
                continue; 
            } else {

                while (player->hasAliveMonsters()) {
                    std::cout << "\nGame is running..." << std::endl;
                    std::cout << "1. Battle" << std::endl;
                    std::cout << "2. Exit to Main Menu" << std::endl;
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
                        std::cout << "1. Easy Grotte (1 Monster - Scaled Down)" << std::endl;
                        std::cout << "2. Medium Grotte (2 Fights in a row - Normal Scaling)" << std::endl;
                        std::cout << "3. Hard Grotte (3 Fights in a row - High Scaling!)" << std::endl;
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
                            
                            // Distribute rewards out of a dynamic loot chance generator
                            int lootRoll = rand() % 5;
                            Item droppedItem = healthPotion;
                            if (lootRoll == 1) droppedItem = poisonDagger;
                            if (lootRoll == 2) droppedItem = iceBomb;
                            if (lootRoll == 3) droppedItem = smokeBomb;
                            if (lootRoll == 4) droppedItem = adrenalineShot;
                            if (lootRoll == 5) droppedItem = jaggedSpear;

                            std::cout << "The dungeon reward chest contained a [" << droppedItem.getName() << "]!" << std::endl;
                            player->addItem(droppedItem); // Transferred securely into character bag
                            std::cout << "Added [" << droppedItem.getName() << "] into your shared inventory!" << std::endl;

                            // POST-BATTLE SQUAD RESTORATION (Wakes monsters up and heals them fully)
                            std::cout << "Your party rests. All team monsters have been revived and fully healed!" << std::endl;
                            for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                const_cast<Monster&>(player->getMonsters()[i]).takeDamage(-100); 
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
                                    // Team full, must choose a replacement target index
                                    std::cout << "Your squad is full (Max 4). Choose a monster to replace:" << std::endl;
                                    for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                        std::cout << i + 1 << ". " << player->getMonsters()[i].getName() 
                                                  << " (HP: " << player->getMonsters()[i].getHealth() 
                                                  << ", Attack: " << player->getMonsters()[i].getAttackPower() << ")" << std::endl;
                                    }
                                    std::cout << "Enter the number of the monster to replace (1-4): ";
                                    std::cin >> replaceIndex;
                                    if (replaceIndex >= 1 && replaceIndex <= 4) {
                                        player->replaceMonster(replaceIndex - 1, lastDefeatedMonster);
                                        std::cout << "Replaced old team member with " << lastDefeatedMonster.getName() << "!" << std::endl;
                                    }
                                }
                            }

                        } else { 
                            std::cout << "\nYou limped out of the cavern ruins in utter defeat..." << std::endl;
                            break; 
                        }        
                    } else if (choiceInsideGame == 2) {
                        std::cout << "Exiting to Main Menu..." << std::endl;
                        break;
                    } else {
                        std::cout << "Invalid choice. Please try again." << std::endl;
                    }
                } // End of while (player->hasAliveMonsters())
            } // End of else (player != nullptr)
            
        } else if (choiceMenu == 2) { 
            std::cout << "Creating a character..." << std::endl;
            std::cout << "Enter character name: ";
            std::cin >> nameInput;
            player = new Character(nameInput);
            
            Monster monster1("Hest", 4, 1);
            Monster monster2("Hest", 4, 1);

            // Give the player a health potion in their inventory from the start
            player->addItem(healthPotion);
            player->addItem(poisonDagger); 
            player->addItem(iceBomb); 
            
            player->addMonster(monster1);
            player->addMonster(monster2);

            std::cout << "Player: " << player->getName() << " has the following monsters:" << std::endl;
            for (const Monster& monster : player->getMonsters()) {
                std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
            }
        } else if (choiceMenu == 3) { 
            std::cout << "Exiting the game. Goodbye!" << std::endl;
            delete player; 
            return 0; 
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } // End of while(true)
    
    return 0;
}