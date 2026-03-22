#include <iostream>
#include "Character.h"
#include "Monster.h"
#include <stdlib.h> // For rand() and srand()
#include <iomanip>

int main(){
    int choiceMenu, choiceInsideGame, choiceBattle, playerTurn, choiceKeepMonster, replaceIndex;
    std::string nameInput;
    
    Character* player = nullptr;
    
    //Enemy monsters
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
                continue; // Prompt the user to create a character first
            } else {

                while (player->hasAliveMonsters()) {
                    std::cout << "Game is running..." << std::endl
                                << "1. Battle" << std::endl
                                << "2. Exit to Main Menu" << std::endl;
                    std::cin >> choiceInsideGame;

                    if (choiceInsideGame == 1) {
                     std::cout << "You have entered a battle!" 
                                << "These are your opponents:" << std::endl; //Show opponents
                                for (size_t i = 0; i < enemyMonsters.size(); ++i) {
                                    std::cout << std::setw(15) << i + 1 << ". " << enemyMonsters[i].getName()
                                              << std::setw(9) << " (HP: " << enemyMonsters[i].getHealth() 
                                              << std::setw(14) <<", Attack: " << enemyMonsters[i].getAttackPower() << ")" << std::endl;
                                }
                                std::cout << "Choose your opponent (1-7)";
                                std::cin >> choiceBattle;
                                std::cout << "You have chosen opponent " << enemyMonsters[choiceBattle - 1].getName() << "!" << std::endl;
                        
                                playerTurn = (rand() % 2) + 1; // Randomly decide who goes first (1 for player, 2 for enemy)

                                while (player->hasAliveMonsters() && enemyMonsters[choiceBattle - 1].isAlive()) {
                                     if (playerTurn == 1) {
                                        std::cout << "Player's turn!" << std::endl;
                                        for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                            if (player->getMonsters()[i].isAlive()) {
                                            const_cast<Monster&>(player->getMonsters()[i]).attack(enemyMonsters[choiceBattle - 1]);
                                        break;
                                        }
                                    }
                                    playerTurn = 2;
                                    } else if (playerTurn == 2) {
                                        std::cout << "Enemy's turn!" << std::endl;
                                        enemyMonsters[choiceBattle - 1].attack(const_cast<Monster&>(player->getMonsters()[0]));
                                        for (size_t i = 0; i < player->getMonsters().size(); ++i) {
                                            if (player->getMonsters()[i].isAlive()) {
                                            enemyMonsters[choiceBattle - 1].attack(const_cast<Monster&>(player->getMonsters()[i]));
                                        break;
                                        }
                                    }
                                    playerTurn = 1;

                            } else if (!player->hasAliveMonsters()) {
                                std::cout << "You have been defeated!" << std::endl;
                                break;

                            } else if (!enemyMonsters[choiceBattle - 1].isAlive()) {
                                std::cout << "You have defeated the enemy!" << std::endl
                                    << "You can now choose to keep the enemys monster" << std::endl
                                    << "1. Yes" << std::endl
                                    << "2. No" << std::endl;
                                    break;
                            }
                            } 
                            std::cin >> choiceKeepMonster; // Ask the player if they want to keep the defeated enemy monster

                            if (choiceKeepMonster == 1) {
                                if (player->getMonsters().size() < 4) { // Check if the player has less than 4 monsters before adding the new one
                                player->addMonster(enemyMonsters[choiceBattle - 1]);
                                std::cout << "You have added " << enemyMonsters[choiceBattle - 1].getName() << " to your collection!" << std::endl;
                                } else { // If the player already has 4 monsters, prompt them to replace one of their existing monsters
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
                    
                } else if (choiceInsideGame == 2) { // Exit to main menu (You are inside the game)
                    std::cout << "Exiting to Main Menu..." << std::endl;
                    break; // Exit to main menu
                } else {
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }     
        break;
        } else if (choiceMenu == 2) { // Character creation
            std::cout << "Creating a character..." << std::endl;
            std::cout << "Enter character name: ";
            std::cin >> nameInput;
            player = new Character(nameInput);
            Monster monster1("Hest", 4, 1);
            Monster monster2("Hest", 4, 1);

            player->addMonster(monster1);
            player->addMonster(monster2);

            std::cout << "Player: " << player->getName() << " has the following monsters:" << std::endl;
            for (const Monster& monster : player->getMonsters()) {
                std::cout << "- " << monster.getName() << " (HP: " << monster.getHealth() << ", Attack: " << monster.getAttackPower() << ")" << std::endl;
            }
        } else if (choiceMenu == 3) { // Exit the game
            std::cout << "Exiting the game. Goodbye!" << std::endl;
            delete player; // Deleting the player object to free memory
            return 0; // Exit the program
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}