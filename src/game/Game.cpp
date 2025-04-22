#include "Game.h"
#include "BattleSystem.h"
#include "../entities/User.h"
#include "../entities/Enemy.h"
#include "../combat/Ability.h"
#include "../combat/Item.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>

Game::Game() : 
    player(nullptr),
    currentArea(1),
    eventCounter(0),
    gameRunning(false) {
    
    std::random_device rd;
    rng = std::mt19937(rd());
    eventDist = std::uniform_int_distribution<int>(0, 99); 
}

Game::~Game() {
    CleanupGame();
}

void Game::Run() {
    InitializeGame();
    gameRunning = true;
    
    std::cout << "==================================================" << std::endl;
    std::cout << "       WELCOME TO TURN-BASED BATTLE ADVENTURE     " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "\nPress Enter to start..." << std::endl;
    std::cin.get();
    
    while (gameRunning) {
        DisplayMainMenu();
        ProcessInput();
    }
    
    std::cout << "\nThank you for playing! Goodbye." << std::endl;
}

void Game::InitializeGame() {
    allAbilities.push_back(Ability::CreateFireball());
    allAbilities.push_back(Ability::CreateHeal());
    allAbilities.push_back(Ability::CreatePoisonStrike());
    allAbilities.push_back(Ability::CreateStunBlow());
    allAbilities.push_back(Ability::CreateIceBlast());
    
    allItems.push_back(Item::CreateSmallPotion());
    allItems.push_back(Item::CreateLargePotion());
    allItems.push_back(Item::CreateRevivePotion());
    allItems.push_back(Item::CreateBomb());
    allItems.push_back(Item::CreateStrengthElixir());
}

void Game::CleanupGame() {
    if (player) {
        delete player;
        player = nullptr;
    }
    
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    

    allAbilities.clear();
    

    allItems.clear();
}

void Game::CreateNewPlayer(const std::string& name) {
    if (player) {
        delete player;
    }
    
    player = new User(name, 100, 10, 5);
    
    player->LearnAbility(allAbilities[0]); //-- THIS IS FIREBALL --//
    player->LearnAbility(allAbilities[1]); //-- THIS IS HEAL--//

    player->AddItem(Item::CreateSmallPotion());
    player->AddItem(Item::CreateSmallPotion());
    
    std::cout << "\nPlayer " << name << " created!" << std::endl;
    std::cout << "HP: " << player->GetHealth() << "/" << player->GetMaxHealth() << std::endl;
    std::cout << "Attack: " << player->GetDamage() << std::endl;
    std::cout << "Defense: " << player->GetDefense() << std::endl;
}

void Game::DisplayMainMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    if (!player) {
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Quit" << std::endl;
    } else {
        DisplayGameMenu();
    }
}

void Game::DisplayGameMenu() {
    std::cout << "\n=== GAME MENU ===" << std::endl;
    std::cout << "Player: " << player->GetName() << " (Level " << player->GetLevel() << ")" << std::endl;
    std::cout << "HP: " << player->GetHealth() << "/" << player->GetMaxHealth() << std::endl;
    std::cout << "EXP: " << player->GetExperience() << "/" << player->GetExpToNextLevel() << std::endl;
    std::cout << "Area: " << currentArea << std::endl;
    std::cout << "\n1. Continue Adventure" << std::endl;
    std::cout << "2. View Inventory" << std::endl;
    std::cout << "3. View Abilities" << std::endl;
    std::cout << "4. Save Game" << std::endl;
    std::cout << "5. Return to Main Menu" << std::endl;
}

void Game::ProcessInput() {
    int choice;
    std::cout << "\nEnter your choice: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
    
    if (!player) {
        switch (choice) {
            case 1: 
                {
                    std::string playerName;
                    std::cout << "\nEnter your character's name: ";
                    std::getline(std::cin, playerName);
                    CreateNewPlayer(playerName);
                }
                break;
                
            case 2: 
                if (!LoadGame("savegame.json")) {
                    std::cout << "Failed to load game. Starting new game..." << std::endl;
                    std::string playerName;
                    std::cout << "\nEnter your character's name: ";
                    std::getline(std::cin, playerName);
                    CreateNewPlayer(playerName);
                }
                break;
                
            case 3:  
                QuitGame();
                break;
                
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } else {
        switch (choice) {
            case 1: 
                {
                    std::cout << "\nYou continue your journey..." << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                    
                    GameEvent event = GenerateRandomEvent();
                    HandleEvent(event);
                }
                break;
                
            case 2: 
                std::cout << "\n=== INVENTORY ===" << std::endl;
                player->ListInventory();
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
                
            case 3:
                std::cout << "\n=== ABILITIES ===" << std::endl;
                player->ListAbilities();
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
                
            case 4: 
                SaveGame("savegame.json");
                std::cout << "Game saved!" << std::endl;
                break;
                
            case 5: 
                delete player;
                player = nullptr;
                std::cout << "Returning to main menu..." << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

GameEvent Game::GenerateRandomEvent() {
    int roll = eventDist(rng);
    
    //-- RANDOM EVENT ODDS --//
    if (roll < 50) {
        return GameEvent::BATTLE;
    } else if (roll < 70) {
        return GameEvent::TREASURE;
    } else if (roll < 85) {
        return GameEvent::HEAL;
    } else if (roll < 95) {
        return GameEvent::TRAP;
    } else {
        return GameEvent::NOTHING;
    }
}

void Game::HandleEvent(GameEvent event) {
    switch (event) {
        case GameEvent::BATTLE:
            StartBattle();
            break;
        case GameEvent::TREASURE:
            FindTreasure();
            break;
        case GameEvent::HEAL:
            FindHealingSpring();
            break;
        case GameEvent::TRAP:
            SpringTrap();
            break;
        case GameEvent::NOTHING:
            std::cout << "You continue walking but nothing interesting happens..." << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
    }
    
    eventCounter++;
    

    //-- WHEN 10 EVENTS HAPPEN, THE AREA CHANGES, BASED ON THIS VARIABLE ENEMIES WILL CHANGE --//
    if (eventCounter % 10 == 0) {
        currentArea++;
        std::cout << "\n*** You've advanced to a new area! ***" << std::endl;
        std::cout << "Welcome to Area " << currentArea << "!" << std::endl;
        std::cout << "Enemies will be stronger here, but rewards will be greater!" << std::endl;
    }
}

void Game::StartBattle() {
    Enemy* enemy = nullptr;
    
    if (currentArea <= 1) {
        enemy = Enemy::CreateGoblin();
    } else if (currentArea <= 3) {
        enemy = Enemy::CreateOrc();
    } else if (currentArea <= 5) {
        enemy = Enemy::CreateDragon();
    } else {
        if (currentArea % 5 == 0) {
            enemy = Enemy::CreateBoss("Area " + std::to_string(currentArea) + " Boss");
        } else {
            enemy = Enemy::CreateDragon();
        }
    }
    
    std::cout << "\nA wild " << enemy->GetName() << " appears!" << std::endl;
    std::cout << "\nPress Enter to start battle...";
    std::cin.get();
    
    BattleSystem battle(player, enemy);
    BattleResult result = battle.StartBattle();

    if (result == BattleResult::PLAYER_DEFEAT) {
        std::cout << "\nYou have been defeated..." << std::endl;
        std::cout << "Game Over" << std::endl;
        
        delete player;
        player = nullptr;
    }
    

    delete enemy;
}

void Game::FindTreasure() {
    std::cout << "\nYou found a treasure chest!" << std::endl;
    
    Item* item = nullptr;
    int roll = eventDist(rng);
    
    if (roll < 50) {
        item = Item::CreateSmallPotion();
    } else if (roll < 80) {
        item = Item::CreateLargePotion();
    } else if (roll < 95) {
        item = Item::CreateBomb();
    } else {
        item = Item::CreateRevivePotion();
    }
    
    player->AddItem(item);
    std::cout << "You got a " << item->GetName() << "!" << std::endl;
    std::cout << item->GetDescription() << std::endl;
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Game::FindHealingSpring() {
    std::cout << "\nYou found a healing spring!" << std::endl;
    
    int healAmount = player->GetMaxHealth() / 2; 
    player->Heal(healAmount);
    
    std::cout << "You drink from the spring and feel refreshed." << std::endl;
    std::cout << "You recovered " << healAmount << " health!" << std::endl;
    std::cout << "HP: " << player->GetHealth() << "/" << player->GetMaxHealth() << std::endl;
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Game::SpringTrap() {
    std::cout << "\nOh no! You triggered a trap!" << std::endl;
    
    int damageAmount = player->GetMaxHealth() / 10; 
    player->TakeDamage(damageAmount);
    
    std::cout << "You take " << damageAmount << " damage!" << std::endl;
    std::cout << "HP: " << player->GetHealth() << "/" << player->GetMaxHealth() << std::endl;
    
    if (!player->IsAlive()) {
        std::cout << "\nYou have died..." << std::endl;
        std::cout << "Game Over" << std::endl;
        
        delete player;
        player = nullptr;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Game::SaveGame(const std::string& filename) {
    if (!player) {
        std::cout << "Error: No player to save!" << std::endl;
        return;
    }
    
    std::ofstream saveFile(filename);
    if (saveFile.is_open()) {
        saveFile << player->GetName() << std::endl;
        saveFile << player->GetHealth() << std::endl;
        saveFile << player->GetMaxHealth() << std::endl;
        saveFile << player->GetDamage() << std::endl;
        saveFile << player->GetDefense() << std::endl;
        saveFile << player->GetLevel() << std::endl;
        saveFile << player->GetExperience() << std::endl;
        saveFile << player->GetExpToNextLevel() << std::endl;

        saveFile << currentArea << std::endl;
        saveFile << eventCounter << std::endl;
        
        saveFile.close();
        std::cout << "Game saved to " << filename << std::endl;
    } else {
        std::cout << "Error: Unable to open save file!" << std::endl;
    }
}

bool Game::LoadGame(const std::string& filename) {
    std::ifstream loadFile(filename);
    if (loadFile.is_open()) {
        std::string playerName;
        int health, maxHealth, damage, defense, level, experience, expToNextLevel;
        
        std::getline(loadFile, playerName);
        loadFile >> health >> maxHealth >> damage >> defense >> level >> experience >> expToNextLevel;
        
        loadFile >> currentArea >> eventCounter;
        
        loadFile.close();
        
        if (player) {
            delete player;
        }
        
        player = new User(playerName, maxHealth, damage, defense);
        
        std::cout << "Game loaded! Welcome back, " << playerName << "!" << std::endl;
        return true;
    } else {
        std::cout << "Error: Unable to open save file!" << std::endl;
        return false;
    }
}

bool Game::IsRunning() const {
    return gameRunning;
}

void Game::QuitGame() {
    std::cout << "Are you sure you want to quit? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    
    if (choice == 'y' || choice == 'Y') {
        gameRunning = false;
    }
}