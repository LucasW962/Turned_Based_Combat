#include "Entity.h"
#include "../combat/Ability.h"
#include "../combat/Item.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Entity::Entity(const std::string& name, int health, int damage, int defense) : 
    name(name),
    health(health),
    maxHealth(health),
    damage(damage),
    defense(defense),
    level(1),
    successRate(85) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        initialized = true;
    }
}

int Entity::Attack(bool success, int amount) {
    if (!success) {
        int roll = rand() % 101; // I rermember it 101 because the start is 0 iirc
        success = (roll <= successRate);
    }
    
    if (success) {
        if (amount <= 0) {
            return damage;
        } else {
            return amount;
        }
    } else {
        std::cout << name << "'s attack missed!" << std::endl;
        return 0;
    }
}

int Entity::TakeDamage(int amount) {
    int actualDamage = amount - defense;
    
    //-- GUARD CLAUSE --//
    if (actualDamage < 0) {
        actualDamage = 0;
    }
    
    health -= actualDamage;
    
    if (health < 0) {
        health = 0;
    }
    
    return actualDamage;
}

int Entity::Heal(int amount) {
    int oldHealth = health;
    health += amount;
    
    if (health > maxHealth) {
        health = maxHealth;
    }
    
    return health - oldHealth;
}

bool Entity::AddItem(Item* item) {
    if (item) {
        inventory.push_back(item);
        return true;
    }
    return false;
}

bool Entity::UseItem(int index) {
    if (index >= 0 && index < static_cast<int>(inventory.size())) {
        Item* item = inventory[index];
        if (item->Use(this)) {
            if (!item->IsUsable()) {
                delete item;
                inventory.erase(inventory.begin() + index);
            }
            return true;
        }
    }
    return false;
}

bool Entity::UseItemWithTarget(int index, Entity* target) {
    if (index >= 0 && index < static_cast<int>(inventory.size())) {
        Item* item = inventory[index];
        if (item->Use(this, target)) {
            if (!item->IsUsable()) {
                delete item;
                inventory.erase(inventory.begin() + index);
            }
            return true;
        }
    }
    return false;
}

size_t Entity::GetInventorySize() const {
    return inventory.size();
}

Item* Entity::GetInventoryItem(int index) const {
    if (index >= 0 && index < static_cast<int>(inventory.size())) {
        return inventory[index];
    }
    return nullptr;
}

void Entity::ListInventory() const {
    if (inventory.empty()) {
        std::cout << "Inventory is empty." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << inventory[i]->GetName() << " (" << inventory[i]->GetUses() << " uses left)" << std::endl;
    }
}

bool Entity::LearnAbility(Ability* ability) {
    if (ability) {
        abilities.push_back(ability);
        return true;
    }
    return false;
}

bool Entity::UseAbility(int index, Entity* target) {
    if (index >= 0 && index < static_cast<int>(abilities.size()) && target) {
        Ability* ability = abilities[index];
        return ability->Use(this, target);
    }
    return false;
}

void Entity::ListAbilities() const {
    if (abilities.empty()) {
        std::cout << "No abilities learned." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < abilities.size(); ++i) {
        std::cout << i + 1 << ". " << abilities[i]->GetName() << " - " << abilities[i]->GetDescription() << std::endl;
    }
}

std::string Entity::GetName() const {
    return name;
}

int Entity::GetHealth() const {
    return health;
}

int Entity::GetMaxHealth() const {
    return maxHealth;
}

int Entity::GetDamage() const {
    return damage;
}

int Entity::GetDefense() const {
    return defense;
}

int Entity::GetLevel() const {
    return level;
}

int Entity::GetSuccessRate() const {
    return successRate;
}

bool Entity::IsAlive() const {
    return health > 0;
}