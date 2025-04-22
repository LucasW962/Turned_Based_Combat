#ifndef KNIGHT_H
#define KNIGHT_H

#include "User.h"

//-- I NEVER GOT TO MAKE THIS WORK BUT IT WAS SUPPOSED TO BE A SUBCLASS OF USER THAT YOU CAN SELECT ON START WITH SPECIAL ABILITIES --//

class Knight : public User {
private:
    int shieldStrength;
    bool isDefending;

public:
    Knight(const std::string& name);
    
    void ShieldBlock();
    int TakeDamage(int amount) override;
    bool UseAbility(int index, Entity* target) override;

    void LearnKnightAbilities();
};

#endif 