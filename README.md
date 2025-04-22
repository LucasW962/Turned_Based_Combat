# Turn-Based Battle Game

-- Finished 4/22/2025, a game that has turn based combat with random events and items

## Game Structure

### Core Game Management

#### Game.h / Game.cpp
The main game controller that manages the overall game flow, including:
  Player creation and management
  Random event generation (battles, treasures, healing springs, traps)
  Area progression
  Game state management (saving/loading)
  Main menu and game menu interfaces

#### BattleSystem.h / BattleSystem.cpp
Handles all combat interactions between players and enemies:
  Turn-based combat mechanics
  Player action selection (attack, abilities, items, running)
  Enemy AI decision making
  Status effect application and tracking
  Battle results and rewards

### Entities

#### Entity.h / Entity.cpp
Base class for all game characters with common attributes and behaviors:
  Health, damage, and defense stats
  Inventory management
  Ability management
  Basic combat actions (attack, take damage, heal)
  Status tracking

#### User.h / User.cpp
Player character class that inherits from Entity, adding:
  Experience points and leveling system
  Player-specific abilities
  User interface interactions

#### Enemy.h / Enemy.cpp
Enemy character class that inherits from Entity, adding:
  Enemy type categorization (Normal, Fire, Water, etc.)
  AI behavior for combat decisions
  Weakness and resistance system
  Experience value and item drop mechanics

#### Knight.h
Specialized player class that inherits from User, adding:
  Shield mechanics and defensive abilities
  Knight-specific combat techniques
  currently cant be used in the main game

### Combat

#### Ability.h / Ability.cpp
Defines special moves and techniques usable in battle:
  Different ability types (Physical, Magical, Healing, etc.)
  Secondary effects (Poison, Burn, Freeze, etc.)
  Success rates and damage calculations


#### Item.h / Item.cpp
Defines usable items with various effects:
  Different item types (Healing, Damage, Buff, etc.)
  Rarity levels affecting drop rates
  Usage mechanics and limits
  Factory methods for creating common items

#### Weapon.h / Weapon.cpp
Defines equipment that can enhance character stats:
  Damage bonuses
  Special effects
  Weapon types and properties

#### CombatAction.h / CombatAction.cpp
Defines the structure of actions performed in combat:
  Action types and priorities
  Targeting system
  Effect implementations


## Build Instructions
// I learned about CMake so let me explain how to compile it
 the demo below is the one i was given to know how to compile
1. Download CMake
2. Clone the repository
3. Create a build directory: "mkdir build"
4. Navigate to the build directory: "cd build"
5. Generate build files with CMake: "cmake ."
6. Build the project: run "ninja"
7. Run the game: (Whatever you have before this)../bin/TurnBasedBattle



## Data Structures

- **Vectors**: Dynamic arrays used for storing entities, items, and abilities, I talked about adding this in the document
- **Pairs**: Used for weaknesses, resistances, and status effect tracking
