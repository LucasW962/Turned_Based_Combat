#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <random>

class User;
class Enemy;
class Item;
class Ability;
class BattleSystem;

enum class GameEvent {
    BATTLE,
    TREASURE,
    HEAL,
    TRAP,
    NOTHING
};

class Game {
private:
    User* player;
    std::vector<Enemy*> enemies;
    std::vector<Item*> allItems;
    std::vector<Ability*> allAbilities;
    
    int currentArea;
    int eventCounter;
    bool gameRunning;
    
    std::mt19937 rng; //-- Rng of 32 bit numbers state size of 19937 bits which is why its named that, pretty neat thing i found on cplusplus.com
    std::uniform_int_distribution<int> eventDist;
    
    // Helper methods
    void InitializeGame();
    void CleanupGame();
    void DisplayMainMenu();
    void DisplayGameMenu();
    void ProcessInput();
    GameEvent GenerateRandomEvent();
    void HandleEvent(GameEvent event);
    void StartBattle();
    void FindTreasure();
    void FindHealingSpring();
    void SpringTrap();
    void SaveGame(const std::string& filename);
    bool LoadGame(const std::string& filename);
    
public:
    Game();
    ~Game();
    
    void Run();
    void CreateNewPlayer(const std::string& name);
    bool IsRunning() const;
    void QuitGame();
};

#endif