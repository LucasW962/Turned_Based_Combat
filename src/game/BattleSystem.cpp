#include "BattleSystem.h"
#include "../entities/User.h"
#include "../entities/Enemy.h"
#include "../combat/Ability.h"
#include "../combat/Item.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <limits>

BattleSystem::BattleSystem(User* player, Enemy* enemy) :
    player(player),
    enemy(enemy),
    result(BattleResult::IN_PROGRESS),
    turnCount(0),
    playerTurn(true),
    battleEnded(false) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        initialized = true;
    }
}

BattleResult BattleSystem::StartBattle() {
    std::cout << "\n=== BATTLE START: " << player->GetName() << " vs " << enemy->GetName() << " ===" << std::endl;
    
    //-- MAIN LOOP WHILE BATTLE IS ACTIVE --//
    while (!battleEnded) {
        DisplayBattleStatus();
        
        if (playerTurn) {
            std::cout << "\n" << player->GetName() << "'s turn. What will you do?" << std::endl;
            DisplayPlayerOptions();
            
            int choice;
            std::cout << "Enter your choice (1-4): ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            
            switch (choice) {
                case 1: 
                    ExecutePlayerAction(BattleAction::ATTACK);
                    break;
                    
                case 2: 
                    {
                        DisplayPlayerAbilities();
                        int abilityIndex;
                        std::cout << "Choose ability (or 0 to go back): ";
                        std::cin >> abilityIndex;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        
                        if (abilityIndex > 0) {
                            ExecutePlayerAction(BattleAction::USE_ABILITY, abilityIndex - 1); 
                        } else {
                            continue;
                        }
                    }
                    break;
                    
                case 3: 
                    {
                        DisplayPlayerItems();
                        int itemIndex;
                        std::cout << "Choose item (or 0 to go back): ";
                        std::cin >> itemIndex;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        
                        if (itemIndex > 0) {
                            ExecutePlayerAction(BattleAction::USE_ITEM, itemIndex - 1); //-- base 0 index --//
                        } else {
                            continue;
                        }
                    }
                    break;
                    
                case 4: 
                    ExecutePlayerAction(BattleAction::RUN);
                    break;
                    
                default:
                    std::cout << "Invalid choice. Try again." << std::endl;
                    continue; 
            }
        } else {
            std::cout << "\n" << enemy->GetName() << "'s turn!" << std::endl;
            ProcessEnemyTurn();
        }
        //-- REMINDER TURN NO LONGER VALID, PLAYER TURN = NOT PLAYER TURN --//
        playerTurn = !playerTurn;
        turnCount++;

        CheckBattleEnd();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    switch (result) {
        case BattleResult::PLAYER_VICTORY:
            std::cout << "\nVictory! " << enemy->GetName() << " was defeated!" << std::endl;
            player->GainExperience(enemy->GetExperienceValue());
            std::cout << player->GetName() << " gained " << enemy->GetExperienceValue() << " experience!" << std::endl;
            
            if (enemy->CalculateDrop()) {
                Item* droppedItem = Item::CreateSmallPotion();
                player->AddItem(droppedItem);
                std::cout << enemy->GetName() << " dropped a " << droppedItem->GetName() << "!" << std::endl;
            }
            break;
            
        case BattleResult::PLAYER_DEFEAT:
            std::cout << "\nDefeat! " << player->GetName() << " was defeated by " << enemy->GetName() << "!" << std::endl;
            std::cout << "Game Over!" << std::endl;
            break;
            
        case BattleResult::ESCAPED:
            std::cout << "\n" << player->GetName() << " escaped from the battle!" << std::endl;
            break;
            
        default:
            break;
    }
    
    return result;
}

bool BattleSystem::ExecutePlayerAction(BattleAction action, int actionIndex) {
    if (!playerTurn || battleEnded) {
        return false;
    }
    
    ProcessPlayerAction(action, actionIndex);
    return true;
}

void BattleSystem::ProcessPlayerAction(BattleAction action, int actionIndex) {
    switch (action) {
        case BattleAction::ATTACK:
            {
                int rawDamage = player->Attack();
                if (rawDamage > 0) {
                    int enemyHealthBefore = enemy->GetHealth();
                    
                    enemy->TakeDamage(rawDamage);
                    
                    int enemyHealthAfter = enemy->GetHealth();
                    int actualDamage = enemyHealthBefore - enemyHealthAfter;
                    
                    std::cout << player->GetName() << " attacks " << enemy->GetName() << " for " << rawDamage << " damage!" << std::endl;
                    
                    if (actualDamage < rawDamage) {
                        std::cout << enemy->GetName() << "'s defense reduced the damage to " << actualDamage << "." << std::endl;
                    }
                } else {
                    std::cout << player->GetName() << "'s attack missed!" << std::endl;
                }
            }
            break;
            
        case BattleAction::USE_ABILITY:
            if (actionIndex >= 0) {
                player->UseAbility(actionIndex, enemy);
            } else {
                std::cout << "No ability selected." << std::endl;
            }
            break;
            
        case BattleAction::USE_ITEM:
            if (actionIndex >= 0) {
                ItemType itemType = ItemType::HEALING;
                if (actionIndex < static_cast<int>(player->GetInventorySize())) {
                    Item* item = player->GetInventoryItem(actionIndex);
                    if (item) {
                        itemType = item->GetType();
                    }
                }

                if (itemType == ItemType::DAMAGE || itemType == ItemType::DEBUFF) {
                    if (player->UseItemWithTarget(actionIndex, enemy)) {
                        std::cout << "Item used successfully on " << enemy->GetName() << "." << std::endl;
                    }
                } else {
                    player->UseItem(actionIndex);
                }
            } else {
                std::cout << "No item selected." << std::endl;
            }
            break;
            
        case BattleAction::RUN:
            {
                int escapeChance = rand() % 100;
                if (escapeChance < 50) {
                    result = BattleResult::ESCAPED;
                    battleEnded = true;
                } else {
                    std::cout << "Couldn't escape!" << std::endl;
                }
            }
            break;
    }
    
    CheckBattleEnd();
}

void BattleSystem::ProcessEnemyTurn() {
    ApplyStatusEffects(enemy, enemyStatusEffects);
    
    if (enemy->IsAlive()) {
        enemy->DecideAction(player);
    }
    
    UpdateStatusEffects(enemyStatusEffects);
}

void BattleSystem::CheckBattleEnd() {
    if (!player->IsAlive()) {
        result = BattleResult::PLAYER_DEFEAT;
        battleEnded = true;
    } else if (!enemy->IsAlive()) {
        result = BattleResult::PLAYER_VICTORY;
        battleEnded = true;
    }
}

void BattleSystem::ApplyStatusEffects(Entity* target, std::vector<std::pair<std::string, int>>& statusEffects) {
    for (auto& effect : statusEffects) {
        if (effect.first == "poison" || effect.first == "burn") {
            int damageOverTime = target->GetMaxHealth() / 10; 
            target->TakeDamage(damageOverTime);
            std::cout << target->GetName() << " takes " << damageOverTime << " damage from "  << effect.first << "!" << std::endl;
        }
    }
}

void BattleSystem::UpdateStatusEffects(std::vector<std::pair<std::string, int>>& statusEffects) {
    for (auto it = statusEffects.begin(); it != statusEffects.end();) {
        it->second--;

        if (it->second <= 0) {
            std::cout << it->first << " effect has worn off!" << std::endl;
            it = statusEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void BattleSystem::DisplayBattleStatus() const {
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << player->GetName() << ": HP " << player->GetHealth() << "/" << player->GetMaxHealth() << std::endl;
    std::cout << enemy->GetName() << ": HP " << enemy->GetHealth() << "/" << enemy->GetMaxHealth() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    if (!playerStatusEffects.empty()) {
        std::cout << player->GetName() << "'s status: ";
        for (const auto& effect : playerStatusEffects) {
            std::cout << effect.first << "(" << effect.second << " turns) ";
        }
        std::cout << std::endl;
    }
    
    if (!enemyStatusEffects.empty()) {
        std::cout << enemy->GetName() << "'s status: ";
        for (const auto& effect : enemyStatusEffects) {
            std::cout << effect.first << "(" << effect.second << " turns) ";
        }
        std::cout << std::endl;
    }
}

void BattleSystem::DisplayPlayerOptions() const {
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. Use Ability" << std::endl;
    std::cout << "3. Use Item" << std::endl;
    std::cout << "4. Run" << std::endl;
}

void BattleSystem::DisplayPlayerAbilities() const {
    std::cout << "Choose an ability:" << std::endl;
    player->ListAbilities();
}

void BattleSystem::DisplayPlayerItems() const {
    std::cout << "Choose an item:" << std::endl;
    player->ListInventory();
}

bool BattleSystem::IsBattleEnded() const {
    return battleEnded;
}

BattleResult BattleSystem::GetBattleResult() const {
    return result;
}

User* BattleSystem::GetPlayer() const {
    return player;
}

Enemy* BattleSystem::GetEnemy() const {
    return enemy;
}

int BattleSystem::GetTurnCount() const {
    return turnCount;
}