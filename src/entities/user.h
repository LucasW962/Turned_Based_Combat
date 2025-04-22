#ifndef USER_H
#define USER_H

#include "Entity.h"
#include <string>
#include <vector>

class Item;
class Ability;

class User : public Entity {
private:
    int experience;
    int expToNextLevel;

public:
    User(const std::string& name, int health, int damage, int defense = 0);

    void GainExperience(int amount);
    bool LevelUp();
    int GetExperience() const;
    int GetExpToNextLevel() const;
    
    bool UseAbility(int index, Entity* target) override;
};

#endif 

