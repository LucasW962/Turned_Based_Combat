#include "Item.h"
#include "../entities/Entity.h"
#include <iostream>

Item::Item(const std::string& name, 
         const std::string& description, 
         ItemType type, 
         ItemRarity rarity, 
         int value, 
         int uses) :
    name(name),
    description(description),
    type(type),
    rarity(rarity),
    value(value),
    uses(uses) {
}

bool Item::Use(Entity* user, Entity* target) {
    if (!IsUsable()) {
        std::cout << "This item cannot be used anymore." << std::endl;
        return false;
    }
    
    bool success = false;
    
    switch (type) {
        case ItemType::HEALING:
            if (user) {
                int healAmount = user->Heal(value);
                std::cout << user->GetName() << " used " << name << " and healed for " << healAmount << " health!" << std::endl;
                success = true;
            }
            break;
            
        case ItemType::MANA:
            std::cout << user->GetName() << " used " << name << " and restored energy!" << std::endl;
            success = true;
            break;
            
        case ItemType::DAMAGE:
            if (target && target->IsAlive()) {
                int targetHealthBefore = target->GetHealth();

                target->TakeDamage(value);
                
                int targetHealthAfter = target->GetHealth();
                int actualDamage = targetHealthBefore - targetHealthAfter;
                
                std::cout << user->GetName() << " used " << name << " on " << target->GetName() << " dealing " << value << " damage!" << std::endl;
                
                if (actualDamage < value) {
                    std::cout << target->GetName() << "'s defense reduced the damage to " << actualDamage << "." << std::endl;
                }
                
                success = true;
            } else {
                std::cout << "No valid target for this item." << std::endl;
            }
            break;
            
        case ItemType::BUFF:
            std::cout << user->GetName() << " used " << name << " and feels stronger!" << std::endl;
            success = true;
            break;
            
        case ItemType::DEBUFF:
            if (target && target->IsAlive()) {
                std::cout << user->GetName() << " used " << name << " on " << target->GetName() << " weakening them!" << std::endl;
                success = true;
            } else {
                std::cout << "No valid target for this item." << std::endl;
            }
            break;
            
        case ItemType::KEY_ITEM:
            std::cout << "This item cannot be used in combat." << std::endl;
            break;
    }
    
    if (success) {
        DecrementUses();
    }
    
    return success;
}

std::string Item::GetName() const {
    return name;
}

std::string Item::GetDescription() const {
    return description;
}

ItemType Item::GetType() const {
    return type;
}

ItemRarity Item::GetRarity() const {
    return rarity;
}

int Item::GetValue() const {
    return value;
}

int Item::GetUses() const {
    return uses;
}

bool Item::IsUsable() const {
    return uses != 0;
}

void Item::DecrementUses() {
    if (uses > 0) {
        uses--;
    }
}


Item* Item::CreateSmallPotion() {
    return new Item(
        "Small Potion",
        "A small potion that restores a little health",
        ItemType::HEALING,
        ItemRarity::COMMON,
        20  
    );
}

Item* Item::CreateLargePotion() {
    return new Item(
        "Large Potion",
        "A large potion that restores a significant amount of health",
        ItemType::HEALING,
        ItemRarity::UNCOMMON,
        50  
    );
}

Item* Item::CreateRevivePotion() {
    return new Item(
        "Revive Potion",
        "A magical potion that can bring an ally back from the brink of death",
        ItemType::HEALING,
        ItemRarity::RARE,
        100
    );
}

Item* Item::CreateBomb() {
    return new Item(
        "Bomb",
        "A small explosive that deals damage to an enemy",
        ItemType::DAMAGE,
        ItemRarity::UNCOMMON,
        30 
    );
}

Item* Item::CreateStrengthElixir() {
    return new Item(
        "Strength Elixir",
        "A powerful elixir that temporarily increases attack power",
        ItemType::BUFF,
        ItemRarity::RARE,
        5, 
        3 
    );
}