#ifndef ENEMEY_H
#define ENEMEY_H

#include "Entity.h"
#include <string>
#include <vector>

enum class EnemyType {
    NORMAL,
    FIRE,
    WATER,
    EARTH,
    WIND,
    BOSS
};

class Enemy : public Entity {
    private:
    EnemyType type;
    int experienceValue;
    std::vector<std::pair<std::string, float>> weaknesses;
    std::vector<std::pair<std::string, float>> resistances;
    float dropRate;

    public:
    Enemy(const std::string& name, int health, int damage, int defense, EnemyType type, int experienceValue, float dropRate = 0.3f);

    void AddWeakness(const std::string& damageType, float multiplier);
    void AddResistance(const std::string& damageType, float multiplier);

    int TakeDamage(int amount) override;

    virtual int DecideAction(Entity* target);

    EnemyType GetType() const;
    int GetExperienceValue() const;
    float GetDropRate() const;
    bool CalculateDrop() const;


    // PUBLIC ENEMY TYPES, ONLY 4 RIGHT NOW BUT IF THERES TIME I PLAN TO MAKE A FEW MORE WITH SECONDARY ABILITIES
    static Enemy* CreateGoblin();
    static Enemy* CreateOrc();
    static Enemy* CreateDragon();
    static Enemy* CreateBoss(const std::string& name);
};


#endif