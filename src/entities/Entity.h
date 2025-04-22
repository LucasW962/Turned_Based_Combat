#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

// THIS CLASS IS IMPORTANT, BOTH USER AND ENEMY INHERIT FROM THIS CLASS

class Item;
class Ability;

class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    int damage;
    int defense;
    int level;
    int successRate;

    std::vector<Item*> inventory;
    std::vector<Ability*> abilities;

public:
    Entity(const std::string& name, int health, int damage, int defense = 0);

    virtual ~Entity() = default;

    virtual int Attack(bool success = false, int amount = 0);
    virtual int TakeDamage(int amount);
    virtual int Heal(int amount);

    // Inventory management
    virtual bool AddItem(Item* item);
    virtual bool UseItem(int index);
    virtual bool UseItemWithTarget(int index, Entity* target);  
    virtual void ListInventory() const;

    size_t GetInventorySize() const;
    Item* GetInventoryItem(int index) const;

    virtual bool LearnAbility(Ability* ability);
    virtual bool UseAbility(int index, Entity* target);
    virtual void ListAbilities() const;

    std::string GetName() const;
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDamage() const;
    int GetDefense() const;
    int GetLevel() const;
    int GetSuccessRate() const;

    bool IsAlive() const;
};

#endif