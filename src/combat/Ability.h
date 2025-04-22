#ifndef ABILITY_H
#define ABILITY_H

#include <string>

enum class AbilityType {
    PHYSICAL,
    MAGICAL,
    HEALING,
    BUFF,
    DEBUFF
};

enum class SecondaryEffect {
    NONE,
    POISON,     
    BURN,    
    FREEZE,   
    PARALYZE,  
    BLIND,     
    REGEN,    
    STUN    
};

class Entity;

class Ability {
private:
    std::string name;
    std::string description;
    int damage;
    int manaCost;
    int successRate;
    AbilityType type;
    bool isRanged;
    SecondaryEffect secondaryEffect;
    int secondaryEffectChance;
    int secondaryEffectPower;   
    int secondaryEffectDuration; //-- SELF REMINDER I ADDED THIS FOR TURN LAST SO I HAVE TO SUBTRACT THIS VALUE PER TURN --//

public:
    Ability(const std::string& name, 
            const std::string& description, 
            int damage, 
            int manaCost, 
            int successRate, 
            AbilityType type, 
            bool isRanged = false, 
            SecondaryEffect secondaryEffect = SecondaryEffect::NONE,
            int secondaryEffectChance = 0,
            int secondaryEffectPower = 0,
            int secondaryEffectDuration = 0);
            
    virtual ~Ability() = default;

    virtual bool Use(Entity* user, Entity* target);
    
    bool ApplySecondaryEffect(Entity* target);

    std::string GetName() const;
    std::string GetDescription() const;
    int GetDamage() const;
    int GetManaCost() const;
    int GetSuccessRate() const;
    AbilityType GetAbilityType() const;
    bool IsRanged() const;
    SecondaryEffect GetSecondaryEffect() const;
    
    static Ability* CreateFireball();
    static Ability* CreateHeal();
    static Ability* CreatePoisonStrike();
    static Ability* CreateStunBlow();
    static Ability* CreateIceBlast();
};

#endif 