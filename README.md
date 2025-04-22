# Turn-Based Battle Game

A text-based battle game written in C++ that uses object-oriented programming principles including inheritance, encapsulation, polymorphism, and abstraction. The game features a combat system similar to Pokémon, with players fighting enemies directly through various abilities and items.

## Game Structure

### Core Game Management

#### Game.h / Game.cpp
The main game controller that manages the overall game flow, including:
- Player creation and management
- Random event generation (battles, treasures, healing springs, traps)
- Area progression
- Game state management (saving/loading)
- Main menu and game menu interfaces

#### BattleSystem.h / BattleSystem.cpp
Handles all combat interactions between players and enemies:
- Turn-based combat mechanics
- Player action selection (attack, abilities, items, running)
- Enemy AI decision making
- Status effect application and tracking
- Battle results and rewards

### Entities

#### Entity.h / Entity.cpp
Base class for all game characters with common attributes and behaviors:
- Health, damage, and defense stats
- Inventory management
- Ability management
- Basic combat actions (attack, take damage, heal)
- Status tracking

#### User.h / User.cpp
Player character class that inherits from Entity, adding:
- Experience points and leveling system
- Player-specific abilities
- User interface interactions

#### Enemy.h / Enemy.cpp
Enemy character class that inherits from Entity, adding:
- Enemy type categorization (Normal, Fire, Water, etc.)
- AI behavior for combat decisions
- Weakness and resistance system
- Experience value and item drop mechanics
- Factory methods for creating common enemies (Goblin, Orc, Dragon, Bosses)

#### Knight.h
Specialized player class that inherits from User, adding:
- Shield mechanics and defensive abilities
- Knight-specific combat techniques

### Combat

#### Ability.h / Ability.cpp
Defines special moves and techniques usable in battle:
- Different ability types (Physical, Magical, Healing, etc.)
- Secondary effects (Poison, Burn, Freeze, etc.)
- Success rates and damage calculations
- Factory methods for creating common abilities

#### Item.h / Item.cpp
Defines usable items with various effects:
- Different item types (Healing, Damage, Buff, etc.)
- Rarity levels affecting drop rates
- Usage mechanics and limits
- Factory methods for creating common items

#### Weapon.h / Weapon.cpp
Defines equipment that can enhance character stats:
- Damage bonuses
- Special effects
- Weapon types and properties

#### CombatAction.h / CombatAction.cpp
Defines the structure of actions performed in combat:
- Action types and priorities
- Targeting system
- Effect implementations

## Game Features

- **Turn-Based Combat**: Strategic battle system where players and enemies take turns
- **Character Progression**: Experience system with level-ups and stat improvements
- **Diverse Abilities**: Various special moves with different effects and types
- **Item System**: Collectible and usable items with different rarities and effects
- **Random Events**: Encounters with enemies, treasure chests, healing springs, and traps
- **Area Progression**: Increasingly difficult areas with stronger enemies and better rewards
- **Save/Load System**: Ability to save game progress and continue later

## Build Instructions

1. Make sure you have CMake (version 3.10+) and a C++ compiler installed
2. Clone the repository
3. Create a build directory: `mkdir build`
4. Navigate to the build directory: `cd build`
5. Generate build files with CMake: `cmake ..`
6. Build the project: `ninja` or `make`
7. Run the game: `../bin/TurnBasedBattle`

## Object-Oriented Programming Features

- **Inheritance**: User and Enemy classes inherit from the Entity base class
- **Encapsulation**: Private data members with public interfaces
- **Polymorphism**: Virtual functions for entity behaviors like Attack and TakeDamage
- **Abstraction**: High-level interfaces hiding implementation details
- **Aggregation**: Game class manages entities, items, and abilities
- **Factory Methods**: Create common game objects through static factory functions

## Data Structures

- **Vectors**: Dynamic arrays used for storing entities, items, and abilities
- **Pairs**: Used for weaknesses, resistances, and status effect tracking