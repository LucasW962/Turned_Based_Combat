#include "Ability.h"
#include "../entities/Entity.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Ability::Ability(const std::string& name, 
        const std::string& description, 
        int damage, 
        int manaCost, 
        int successRate, 
        AbilityType type, 
        bool isRanged, 
        SecondaryEffect secondaryEffect,
        int secondaryEffectChance,
        int secondaryEffectPower,
        int secondaryEffectDuration) : 
    name(name),
    description(description),
    damage(damage),
    manaCost(manaCost),
    successRate(successRate),
    type(type),
    isRanged(isRanged),
    secondaryEffect(secondaryEffect),
    secondaryEffectChance(secondaryEffectChance),
    secondaryEffectPower(secondaryEffectPower),
    secondaryEffectDuration(secondaryEffectDuration) {
    
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        initialized = true;
    }
}

bool Ability::Use(Entity* user, Entity* target) {
    if (!user || !target || !target->IsAlive()) {
        return false;
    }
    
    int roll = rand() % 101; //-- 101 because it  starts at 0 i believe --//
    bool success = (roll <= successRate);
    
    if (!success) {
        std::cout << user->GetName() << " tried to use " << name << " but failed!" << std::endl;
        return false;
    }
    
    switch (type) {
        case AbilityType::PHYSICAL:
        case AbilityType::MAGICAL:
            {
                int damageDealt = target->TakeDamage(damage);
                std::cout << user->GetName() << " used " << name << " on " << target->GetName() << " dealing " << damageDealt << " damage!" << std::endl;
                
                ApplySecondaryEffect(target);
            }
            break;
            // These cases dont actually do much right now, only secondary effect does
        case AbilityType::HEALING:
            {
                int healAmount = user->Heal(damage); 
                std::cout << user->GetName() << " used " << name << " and healed for " << healAmount << " health!" << std::endl;
            }
            break;
            
        case AbilityType::BUFF:

            std::cout << user->GetName() << " used " << name << " to boost their stats!" << std::endl;
            break;
            
        case AbilityType::DEBUFF:
            std::cout << user->GetName() << " used " << name << " to weaken " << target->GetName() << "!" << std::endl;
            ApplySecondaryEffect(target);
            break;
    }
    
    return true;
}

bool Ability::ApplySecondaryEffect(Entity* target) {
    if (secondaryEffect == SecondaryEffect::NONE || !target) {
        return false;
    }
    
    int roll = rand() % 101;
    if (roll > secondaryEffectChance) {
        return false;
    }
    
    switch (secondaryEffect) {
        case SecondaryEffect::POISON:
            std::cout << target->GetName() << " was poisoned! They'll take damage over time." << std::endl;
            break;
        case SecondaryEffect::BURN:
            std::cout << target->GetName() << " was burned! They'll take damage over time." << std::endl;
            break;
        case SecondaryEffect::FREEZE:
            std::cout << target->GetName() << " was frozen! They might miss their next turn." << std::endl;
            break;
        case SecondaryEffect::PARALYZE:
            std::cout << target->GetName() << " was paralyzed! Their accuracy is reduced." << std::endl;
            break;
        case SecondaryEffect::BLIND:
            std::cout << target->GetName() << " was blinded! Their accuracy is severely reduced." << std::endl;
            break;
        case SecondaryEffect::STUN:
            std::cout << target->GetName() << " was stunned! They'll miss their next turn." << std::endl;
            break;
        case SecondaryEffect::REGEN:
            std::cout << target->GetName() << " feels regenerative energy! They'll heal over time." << std::endl;
            break;
        default:
            return false;
    }
    
    return true;
}

std::string Ability::GetName() const {
    return name;
}

std::string Ability::GetDescription() const {
    return description;
}

int Ability::GetDamage() const {
    return damage;
}

int Ability::GetManaCost() const {
    return manaCost;
}

int Ability::GetSuccessRate() const {
    return successRate;
}

AbilityType Ability::GetAbilityType() const {
    return type;
}

bool Ability::IsRanged() const {
    return isRanged;
}

SecondaryEffect Ability::GetSecondaryEffect() const {
    return secondaryEffect;
}

//-- CONSTRUCTED ABILITY WITH STATS BELOW --//

Ability* Ability::CreateFireball() {
    return new Ability(
        "Fireball",
        "A ball of fire that deals damage and may cause burning",
        25, 
        15, 
        90, 
        AbilityType::MAGICAL,
        true, 
        SecondaryEffect::BURN,
        30,  
        5,  
        3   
    );
}

Ability* Ability::CreateHeal() {
    return new Ability(
        "Heal",
        "A healing spell that restores health",
        20, 
        20,
        100, 
        AbilityType::HEALING
    );
}

Ability* Ability::CreatePoisonStrike() {
    return new Ability(
        "Poison Strike",
        "A poisoned blade that deals damage and may poison the target",
        15, 
        10, 
        85, 
        AbilityType::PHYSICAL,
        false, 
        SecondaryEffect::POISON,
        40,  
        3,   
        4    
    );
}

Ability* Ability::CreateStunBlow() {
    return new Ability(
        "Stun Blow",
        "A powerful strike that may stun the target",
        20, 
        15,
        75, 
        AbilityType::PHYSICAL,
        false, 
        SecondaryEffect::STUN,
        25,
        1,  
        1   
    );
}

Ability* Ability::CreateIceBlast() {
    return new Ability(
        "Ice Blast",
        "A blast of freezing cold that damages and may freeze the target",
        20, 
        20, 
        85, 
        AbilityType::MAGICAL,
        true, 
        SecondaryEffect::FREEZE,
        20,  
        1,   
        1   
    );
}