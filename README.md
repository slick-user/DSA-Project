# 🎮 Project Feature Roadmap & To-Do List

This document tracks all planned, in-progress, and completed features for the game project.  
Use this to coordinate development and ensure all modules stay consistent across the system.

---

## ✅ Legend
- [ ] To Do  
- [~] In Progress  
- [x] Completed  

---

# **1. Main Menu System**

## Start Menu
- [x] Start Game
- [x] Select Level
- [x] Leaderboard
- [x] Multiplayer Mode Selection
- [x] Profile Section Access

## End Menu
- [~] End the game cleanly (resource cleanup, save score)
- [x] Display final score
- [x] Highlight new high score
- [x] Options: Restart, Main Menu, Exit Game

## Additional Menu Features
- [ ] Settings Menu (Sound, Controls, Themes)
- [~] Theme Selector (connected to AVL Theme Inventory)
- [?] Tutorial / How to Play
- [?] Credits Page

---

# **2. Login & Authentication System**

## Registration
- [x] Register new users with:
  - [x] Unique username
  - [x] Secure password
  - [x] Optional nickname, email, etc.
- [x] Input validation:
  - [x] Username uniqueness check
  - [x] Password strength check (min length)
- [x] Data storage:
  - [x] Save credentials to file
  - [x] Store registration timestamp / Player ID
  - [x] Structured, readable file format

## Login
- [x] Validate username exists
- [x] Validate password is correct
- [x] Allow entry to player features upon successful login

---

# **3. Points System & Power-Ups**

## Scoring Rules
- [x] +1 point per tile captured
- [x] Double points (×2) when >10 tiles captured in one move
- [x] Reward counter:
  - [x] After 3 bonuses → threshold becomes 5 tiles
  - [x] After 5 bonuses → >5 tiles = ×4 points

## Power-Ups
- [x] Award power-up at score 50
- [x] Additional power-ups at 70, 100, 130, +every 30 points
- [x] Power-ups pause all enemies for 3 seconds
- [x] Power-ups stack in inventory
- [x] Show available power-ups on screen

---

# **4. Player Profile System**

Each profile includes:
- [x] Username
- [x] Friends List
- [x] Match History (Win/Loss)
- [x] Total Points
- [ ] Saved Theme Preference
- [ ] Saved Games List (via Save System)

Operations:
- [x] View profile
- [x] Update profile data
- [ ] Sync with Leaderboard and Matchmaking Queue

---

# **5. Multiplayer Mode**

## Game Logic
- [x] Support two-player mode on shared board
- [x] Player controls:
  - [x] Player 1: Arrow Keys
  - [x] Player 2: Q, G, A, X
- [x] Separate scores & power-ups displayed on screen

## Collision Rules
- [x] Player–player collision handling:
  - [x] If both constructing → both die
  - [x] If P1 touches P2 constructing → P1 dies (and vice versa)
  - [x] If constructing vs not constructing → constructor dies
- [x] Power-up affects enemies AND the other player in multiplayer

## Win Conditions
- [x] Game ends when both players die
- [x] Highest score wins

---

# **6. Leaderboard (Min-Heap)**

## Data Structure
- [x] Implement Min-Heap of size 10
- [x] Root always contains lowest score among top 10

## Updating
- [x] Insert new players if leaderboard not full
- [x] Replace root if new score > current minimum
- [x] Heapify after insertion/removal
- [x] Sync scores with Player Profiles

## Display
- [x] Sort temporarily in descending order for display
- [x] Show Player ID & Total Score

---

# **7. Matchmaking Queue (Priority Queue)**

- [ ] Use priority queue based on player total score
- [ ] Higher score = higher priority
- [ ] Always match top two players
- [ ] After matches, update scores & re-insert players as necessary
- [ ] Automatically form game rooms from queue

---

# **8. Game Room System**

- [ ] Use queue structure to store players waiting for a match
- [ ] Pair players automatically as soon as 2 are available
- [ ] Connect with multiplayer game session
- [ ] Sync with Matchmaking Priority Queue

---

# **9. Friend System (Array List + Linked List + Hash Table)**

## Player List (Array List)
- [x] Store player objects in dynamic array
- [x] Each player contains:
  - [x] Username / Player ID
  - [x] Linked list of friends

## Hash Table
- [x] Map username → index in array list
- [ ] Fast lookup for:
  - [ ] Sending friend requests
  - [ ] Accepting requests
  - [ ] Searching players

## Friend Request Operations
- [x] Send friend request
- [x] Accept or reject request
- [x] Update linked lists for both players
- [x] View friend list

---

# **10. Save Game System**

## Save Structure
- [x] Create `GameState` structure/class with:
  - [x] Timestamp
  - [x] Player ID
  - [x] Linked list of tiles created so far
  - [x] Current score + power-ups

## Save / Load
- [x] Serialize `GameState` to file
- [x] Assign unique Save ID
- [x] Load by entering Save ID
- [x] Reconstruct game state fully
- [x] Resume game from saved point

---

# **11. Inventory System (AVL Tree Themes)**

## Initialization
- [x] Load predefined themes into AVL Tree
- [x] Each theme contains:
  - [x] Theme ID
  - [x] Theme Name
  - [x] Description
  - [x] Optional preview (color code or text)

## Tree Functionality
- [x] Insert theme
- [x] Search theme by ID
- [x] In-order traversal (sorted display)
- [x] Keep tree balanced (rotations)
- [x] Optional update/delete features

## Theme Selection
- [x] Implement Themes
- [ ] Browse themes in sorted order
- [ ] Search & apply theme by ID
- [ ] Save selected theme to player profile

---

# **12. General Enhancements & Polishing**
- [x] Full UI pass (consistent, clean)
- [x] File-handling error checking everywhere
- [?] Add sound effects toggle
- [~] Adjustable difficulty levels
- [ ] Logs for debugging crashes or unexpected behavior

