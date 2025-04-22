#ifndef ITEM_H
#define ITEM_H

#include <string>


class Entity;

enum class ItemType {
    HEALING,        
    MANA,         
    DAMAGE,      
    BUFF,        
    DEBUFF,        
    KEY_ITEM        
};

enum class ItemRarity {
    COMMON,     
    UNCOMMON,    
    RARE,          
    EPIC,      
    LEGENDARY    
};

class Item {
private:
    std::string name;
    std::string description;
    ItemType type;
    ItemRarity rarity;
    int value;    
    int uses;   
    
public:
    // Constructor
    Item(const std::string& name, 
         const std::string& description, 
         ItemType type, 
         ItemRarity rarity, 
         int value, 
         int uses = 1);
    
    virtual ~Item() = default;
    
    virtual bool Use(Entity* user, Entity* target = nullptr);
    

    std::string GetName() const;
    std::string GetDescription() const;
    ItemType GetType() const;
    ItemRarity GetRarity() const;
    int GetValue() const;
    int GetUses() const;
    
    bool IsUsable() const;
    
    void DecrementUses();
    
    static Item* CreateSmallPotion();
    static Item* CreateLargePotion();
    static Item* CreateRevivePotion();
    static Item* CreateBomb();
    static Item* CreateStrengthElixir();
};

#endif 