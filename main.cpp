#include <iostream>
#include "Character.h"
#include "Monster.h"
#include <stdlib.h> // For rand() and srand()
#include <iomanip>
#include "BattleEngine.h"

int main(){
    int choiceMenu, choiceInsideGame, choiceBattle, playerTurn, choiceKeepMonster, replaceIndex;
    std::string nameInput;
    
    Character* player = nullptr;
    
    // Create a pool of concrete items tied to your status effects
    // Format: Item(name, damage, statusType, duration, chance)
    Item healthPotion("Health Potion", -15, StatusType::NONE, 0, 0);       // Heals your active monster
    Item antidote("Antidote", 0, StatusType::NONE, 0, 0);                  // Cures poison and bleeding statuses
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
                    std::cout << "\nGame is running..." << std::endl
                                << "1. Battle" << std::endl
                                << "2. Exit to Main Menu" << std::endl;
                    std::cin >> choiceInsideGame;

                    if (choiceInsideGame == 1) {
                        // Team and Inventory Display: Show player's current monsters and items before battle to help them strategize
                        std::cout << "\n--- YOUR TEAM ---" << std::endl;
                        for (const Monster& monster : player->getMonsters()) {
                            std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
                        }

                        std::cout << "--- YOUR INVENTORY ---" << std::endl;
                        if (player->getItems().empty()) {
                            std::cout << " [Empty]" << std::endl;
                        } else {
                            for (size_t i = 0; i < player->getItems().size(); ++i) {
                                std::cout << " - " << player->getItems()[i].getName() << std::endl;
                            }
                        }
                        // Enemy Selection: Let the player choose which enemy monster to battle from the pool of 7, each with different stats and difficulty
                        std::cout << "You have entered a battle! These are your opponents:" << std::endl;
                        for (size_t i = 0; i < enemyMonsters.size(); ++i) {
                            std::cout << std::setw(15) << i + 1 << ". " << enemyMonsters[i].getName()
                                      << std::setw(9) << " (HP: " << enemyMonsters[i].getHealth() 
                                      << std::setw(14) << ", Attack: " << enemyMonsters[i].getAttackPower() << ")" << std::endl;
                        }
                        std::cout << "Choose your opponent (1-7): ";
                        std::cin >> choiceBattle;
                        
                        if (choiceBattle < 1 || choiceBattle > 7) {
                            std::cout << "Invalid opponent selection!" << std::endl;
                            continue;
                        }

                        Monster activeEnemy = enemyMonsters[choiceBattle - 1];   

                        bool playerWon = BattleEngine::startBattle(*player, activeEnemy);

                        if (playerWon) {
                            std::cout << "\nCongratulations! You defeated " << activeEnemy.getName() << "!" << std::endl;
                            
                            // LOOT SYSTEM: Reward the player with an item upon winning
                            int lootRoll = rand() % 4;
                            Item droppedItem = healthPotion; // Default drop
                            if (lootRoll == 1) droppedItem = poisonDagger;
                            if (lootRoll == 2) droppedItem = adrenalineShot;
                            if (lootRoll == 3) droppedItem = jaggedSpear;

                            std::cout << "The defeated monster dropped a [" << droppedItem.getName() << "]!" << std::endl;
                            
                            std::cout << "The defeated monster dropped a [" << droppedItem.getName() << "]!" << std::endl;
                            player->addItem(droppedItem); // Adds directly to player inventory!
                            std::cout << "Added [" << droppedItem.getName() << "] to your inventory bag!" << std::endl;

                            // POST-BATTLE HEALING: Revive and fully heal all player monsters so progression is sustainable
                            std::cout << "Your team rests after the battle. All your monsters have been fully revived and healed!" << std::endl;
                            for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                // We reset them to 4 HP since that's your starter monster base health
                                const_cast<Monster&>(player->getMonsters()[i]).reviveAndHeal(4); 
                            }

                            // Capture mechanics
                            std::cout << "\nYou can now choose to keep the enemy's monster:" << std::endl
                                      << "1. Yes" << std::endl
                                      << "2. No" << std::endl;
                            std::cin >> choiceKeepMonster;

                            if (choiceKeepMonster == 1) {
                                if (player->getMonsters().size() < 4) { 
                                    player->addMonster(enemyMonsters[choiceBattle - 1]);
                                    std::cout << "You have added " << enemyMonsters[choiceBattle - 1].getName() << " to your collection!" << std::endl;
                                } else { 
                                    std::cout << "You already have 4 monsters. Please replace one of your existing monsters." << std::endl;
                                    std::cout << "Your current monsters:" << std::endl;
                                    for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                        std::cout << i + 1 << ". " << player->getMonsters()[i].getName() 
                                                  << " (HP: " << player->getMonsters()[i].getHealth() 
                                                  << ", Attack: " << player->getMonsters()[i].getAttackPower() << ")" << std::endl;
                                    }
                                    std::cout << "Enter the number of the monster you want to replace (1-4): ";
                                    std::cin >> replaceIndex;
                                    if (replaceIndex >= 1 && replaceIndex <= 4) {
                                        player->replaceMonster(replaceIndex - 1, enemyMonsters[choiceBattle - 1]);
                                        std::cout << "You have replaced your monster with " << enemyMonsters[choiceBattle - 1].getName() << "!" << std::endl;
                                    } else {
                                        std::cout << "Invalid choice. You did not replace any monster." << std::endl;
                                    }
                                }
                            } else {
                                std::cout << "You chose not to keep the monster." << std::endl;
                            }

                        } else { 
                            std::cout << "You were defeated by " << activeEnemy.getName() << ". Better luck next time!" << std::endl;
                            break; 
                        }
                    } else if (choiceInsideGame == 2) { 
                        std::cout << "Exiting to Main Menu..." << std::endl;
                        break; 
                    } else {
                        std::cout << "Invalid choice. Please try again." << std::endl;
                    }
                }
            }     
            break;
        } else if (choiceMenu == 2) { 
            std::cout << "Creating a character..." << std::endl;
            std::cout << "Enter character name: ";
            std::cin >> nameInput;
            player = new Character(nameInput);
            
            Monster monster1("Hest", 4, 1);
            Monster monster2("Hest", 4, 1);

            // Give the player a health potion in their inventory from the start
            player->addItem(healthPotion);
            
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
    }
    
    return 0;
}