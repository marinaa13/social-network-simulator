# Social Network Simulator

A social media simulation implemented in **C**, combining user management, friendship graphs, post/repost systems, and user feed generation.  
The program integrates graph traversal, BFS algorithms, and data structures such as linked lists and trees to emulate core social platform features.

---

## Overview

The project models a simplified social network divided into three main components:

1. **Users and Friends** – manages connections between accounts  
2. **Posts and Reposts** – handles content creation and interaction  
3. **Social Media Integration** – combines both systems for feed and profile simulation  

Each command follows a consistent structure:
- Parse input arguments  
- Execute the corresponding operation  
- Print results or success messages  

---

## Features

### 1. Users and Friends
Implements the social graph using an **adjacency matrix**.  
Supports:
- Adding or removing friends  
- Computing shortest distance between two users (via BFS)  
- Suggesting friends of friends  
- Finding common friends  
- Counting friends per user  
- Determining the most popular account  

Key functions:
- `add_friend()`, `remove_friend()`  
- `distance_friend()` — uses BFS for shortest path  
- `suggestions_friend()` — finds second-degree connections  
- `common_people()` — intersection of friend lists  
- `friends()` — counts user connections  
- `influencer()` — finds the account with most friends  

---

### 2. Posts and Reposts
Implements user-generated content using:
- A **doubly linked list** for posts  
- A **tree structure** for reposts  

Supports:
- Creating and deleting posts  
- Reposting existing content  
- Liking and unliking posts  
- Computing ratios between posts and reposts  
- Viewing repost trees and like counts  

Key algorithms:
- Tree traversal for reposts  
- Parent tracking for common repost origins  
- Array-based like tracking per post  

---

### 3. Social Media Integration
Combines both modules into a unified simulation of:
- **User feed** — retrieves most recent posts by user and friends  
- **Profile view** — shows a user’s posts in chronological order  
- **Friends that reposted** — lists a user’s friends who reposted a given post  
- **Clique detection** — finds maximal cliques using the **Bron–Kerbosch algorithm**  

The recursive Bron–Kerbosch algorithm computes all maximal friend groups, using sets:
- `R` — current clique  
- `P` — potential candidates  
- `X` — excluded vertices  

---

## Example Commands

```
add_friend user1 user2
remove_friend user3 user4
create_post user1 "Hello world!"
repost user2 3
like user3 3
feed user1 5
view_profile user2
friends_that_reposted user1 3
bk
```
---
## Learning Outcomes
- Applied advanced data structures (graphs, lists, trees)  
- Implemented BFS and Bron–Kerbosch graph algorithms  
- Managed dynamic memory safely in complex interlinked structures  
- Simulated realistic interactions between users and posts in C  
