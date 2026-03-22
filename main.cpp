#include <iostream>
#include "Character.h"
#include "Monster.h"

int main(){
    int choice;
    std::string nameInput;
    
    Character* player = nullptr;

    while(true) {
        std::cout << "Welcome to the Monster Game!" << std::endl;
        std::cout << "1. Start New Game" << std::endl;
        std::cout << "2. Make character" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Starting a new game..." << std::endl;
            if (player == nullptr) {
                std::cout << "Please create a character before starting the game." << std::endl;
                continue; // Prompt the user to create a character first
            } else {

                // GAME LOGIC GOES HERE
                
            break;} 
        } else if (choice == 2) {
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
        } else if (choice == 3) {
            std::cout << "Exiting the game. Goodbye!" << std::endl;
            delete player; // Deleting the player object to free memory
            return 0; // Exit the program
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}