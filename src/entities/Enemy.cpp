#include "Enemy.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>


Enemy::Enemy(const std::string& name, int health, int damage, int defense, EnemyType type, int experienceValue, float dropRate) : 
    Entity(name, health, damage, defense),
    type(type),
    experienceValue(experienceValue),
    dropRate(dropRate) {
        switch (type) {
            case EnemyType::BOSS:
                successRate = 95; 
                break;
            case EnemyType::NORMAL:
                successRate = 85; 
                break;
            default:
                successRate = 75; 
                break;
        }
    }

void Enemy::AddWeakness(const std::string& damageType, float multiplier) {
    if (multiplier > 1.0f) {
        weaknesses.push_back(std::make_pair(damageType, multiplier));
    }
}

void Enemy::AddResistance(const std::string& damageType, float multiplier) {
    if (multiplier < 1.0f) {
        resistances.push_back(std::make_pair(damageType, multiplier));
    }
}

int Enemy::TakeDamage(int amount) {
    return Entity::TakeDamage(amount);
    
    /* FOR FUTURE IF I HAVE TIME, ADD ITEM DAMAGE

    SECONDAY AND EFFECT DAMAGE IF POSSIBLE
    */
}

//-- THIS IS FOR THE ENEMY IT MIMICS A REAL PERSON USING RNG TO DECIDE WHAT ACTION TO TAKE --//
int Enemy::DecideAction(Entity* target) {
    if (!target || !target->IsAlive()) {
        return 0;
    }
    
    int decision = rand() % 101;
    
    if (decision < 70) {
        int rawDamage = Attack();
        if (rawDamage > 0) {
            int targetHealthBefore = target->GetHealth();
            
            target->TakeDamage(rawDamage);
            
            int targetHealthAfter = target->GetHealth();
            int actualDamage = targetHealthBefore - targetHealthAfter;
            
            std::cout << name << " attacks " << target->GetName() << " for " << rawDamage << " damage!" << std::endl;
            
            if (actualDamage < rawDamage) {
                std::cout << target->GetName() << "'s defense reduced the damage to " << actualDamage << "." << std::endl;
            }
            
            return actualDamage;
        }
        std::cout << name << "'s attack missed!" << std::endl;
        return 0;
    }
    else if (decision < 85 && !abilities.empty()) //-- MAKE SURE ABILITES ARE *NOT* EMPTY IF ITS ROLLED --// 
    {
        int abilityIndex = rand() % abilities.size();
        std::cout << name << " uses a special ability!" << std::endl;
        UseAbility(abilityIndex, target);
        return 0; 
    }
    else if (decision < 95 && health < maxHealth / 2) {
        int healAmount = maxHealth / 10;
        Heal(healAmount);
        std::cout << name << " heals for " << healAmount << " health!" << std::endl;
        return -healAmount;
    }
    else {
        std::cout << name << " hesitates and does nothing!" << std::endl;
        return 0;
    }
}

EnemyType Enemy::GetType() const {
    return type;
}

int Enemy::GetExperienceValue() const {
    return experienceValue;
}

float Enemy::GetDropRate() const {
    return dropRate;
}

bool Enemy::CalculateDrop() const {
    int roll = rand() % 100;
    return (roll < static_cast<int>(dropRate * 100));
}

Enemy* Enemy::CreateGoblin() {
    Enemy* goblin = new Enemy("Goblin", 30, 5, 2, EnemyType::NORMAL, 10, 0.3f);
    goblin->AddWeakness("fire", 1.5f);
    return goblin;
}

Enemy* Enemy::CreateOrc() {
    Enemy* orc = new Enemy("Orc", 60, 8, 4, EnemyType::NORMAL, 20, 0.4f);
    orc->AddWeakness("magic", 1.3f);
    orc->AddResistance("physical", 0.8f);
    return orc;
}

Enemy* Enemy::CreateDragon() {
    Enemy* dragon = new Enemy("Dragon", 150, 15, 8, EnemyType::FIRE, 50, 0.6f);
    dragon->AddWeakness("water", 2.0f);
    dragon->AddResistance("physical", 0.7f);
    dragon->AddResistance("fire", 0.1f); 
    return dragon;
}

Enemy* Enemy::CreateBoss(const std::string& name) {
    Enemy* boss = new Enemy(name, 300, 20, 12, EnemyType::BOSS, 100, 1.0f); 
    boss->AddResistance("physical", 0.5f);
    boss->AddResistance("magic", 0.7f);
    return boss;
}