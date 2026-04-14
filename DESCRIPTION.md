# Castle Defender

A **grid-based tower defense game in C++** featuring adaptive AI, multiple enemy types, and a real-time simulation loop.

Built as a university project with a focus on **object-oriented design** and **AI behaviour**.

> **Note:** This project uses Windows-specific console features (`<windows.h>`).  
> It may require modification to run on Linux/macOS.

---

## Gameplay

- The game runs on a **20×20 grid**
- Player places **up to 5 towers**
- Enemies spawn at the top and move toward the castle
- Towers attack automatically each turn
- AI adapts difficulty based on performance

---

## Core Mechanics

### Castle
- Located at bottom center
- Starts with **100 HP**
- Each enemy reaching it deals **10 damage**

---

### Towers
- Range-based attacks (`|r−er| ≤ range`)
- Deal damage + **splash damage (3×3 area)**
- Gain XP and **level up dynamically**
- Stats scale with level (damage + range)

---

### Enemies

- **Enemy** – base unit  
- **FastEnemy** – faster, lower health  
- **StrongEnemy** – high health tank  

Each enemy:
- Moves toward the castle
- Uses diagonal movement if blocked
- Has unique stats and behavior

---

## AI System

### Spawning
- Tracks where enemies die
- Avoids dangerous columns using weighted randomness

### Adaptive Difficulty

```cpp
if (player_score >= threshold)
    enemyBaseHealth++;
