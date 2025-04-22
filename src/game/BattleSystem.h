#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include <vector>
#include <string>
#include <functional>

class Entity;
class User;
class Enemy;

enum class BattleResult {
    IN_PROGRESS,
    PLAYER_VICTORY,
    PLAYER_DEFEAT,
    ESCAPED
};

enum class BattleAction {
    ATTACK,
    USE_ABILITY,
    USE_ITEM,
    RUN
};

class BattleSystem {
private:
    User* player;
    Enemy* enemy;
    BattleResult result;
    int turnCount;
    bool playerTurn;
    bool battleEnded;
    
    std::vector<std::pair<std::string, int>> playerStatusEffects;  
    std::vector<std::pair<std::string, int>> enemyStatusEffects;
    
    // Helper methods
    void ProcessTurn();
    void ProcessPlayerAction(BattleAction action, int actionIndex = -1);
    void ProcessEnemyTurn();
    void CheckBattleEnd();
    void ApplyStatusEffects(Entity* target, std::vector<std::pair<std::string, int>>& statusEffects);
    void UpdateStatusEffects(std::vector<std::pair<std::string, int>>& statusEffects);
    void DisplayBattleStatus() const;

public:
    BattleSystem(User* player, Enemy* enemy);
    
    BattleResult StartBattle();
    
    bool ExecutePlayerAction(BattleAction action, int actionIndex = -1);
    
    bool IsBattleEnded() const;
    BattleResult GetBattleResult() const;

    User* GetPlayer() const;
    Enemy* GetEnemy() const;
    int GetTurnCount() const;
    
    void DisplayPlayerOptions() const;
    void DisplayPlayerAbilities() const;
    void DisplayPlayerItems() const;
};

#endif