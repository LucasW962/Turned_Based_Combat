#include "user.h"
#include "../combat/Ability.h"
#include "../combat/Item.h"
#include <iostream>
#include <cmath>

User::User(const std::string& name, int health, int damage, int defense)
    : Entity(name, health, damage, defense),
      experience(0),
      expToNextLevel(100) {
    successRate = 90; 
}

void User::GainExperience(int amount) {
    if (amount <= 0) {
        return;
    }
    
    experience += amount;
    std::cout << name << " gained " << amount << " experience!" << std::endl;
    
    if (experience >= expToNextLevel) {
        LevelUp();
    }
}

bool User::LevelUp() {
    if (experience < expToNextLevel) {
        return false;
    }
    
    level++;
    
    maxHealth += 10;
    health = maxHealth; 
    damage += 2;
    defense += 1;
    
    experience -= expToNextLevel;
    expToNextLevel = static_cast<int>(expToNextLevel * 1.5);
    
    std::cout << "\n*** " << name << " LEVEL UP! ***" << std::endl;
    std::cout << "Level " << level << " reached!" << std::endl;
    std::cout << "Max health increased to " << maxHealth << "!" << std::endl;
    std::cout << "Damage increased to " << damage << "!" << std::endl;
    std::cout << "Defense increased to " << defense << "!" << std::endl;
    
    if (experience >= expToNextLevel) {
        return LevelUp();
    }
    
    return true;
}

int User::GetExperience() const {
    return experience;
}

int User::GetExpToNextLevel() const {
    return expToNextLevel;
}

bool User::UseAbility(int index, Entity* target) {
    return Entity::UseAbility(index, target);
}

