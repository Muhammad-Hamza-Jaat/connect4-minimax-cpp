# Connect 4 AI — Minimax with Alpha-Beta Pruning (C++)

A terminal-based **Connect 4** game written in C++ where you play against an AI opponent powered by a **3-ply Minimax look-ahead search with Alpha-Beta pruning**[cite: 30, 44]. Built using pure procedural C++ (`<iostream>`) and 2D character arrays—no recursion, STL containers, or external libraries[cite: 14, 44].

---

## How to Play

* **Board:** A `6 × 7` grid with columns numbered `0` to `6` from left to right[cite: 30, 44].
* **Turns:** You (`X`) move first, and the computer (`O`) replies immediately after[cite: 30, 44].
* **Controls:** Enter a column number (`0–6`) and press `Enter`[cite: 30, 44]. Your piece drops to the lowest available row in that column[cite: 30, 44]. Invalid or full columns are rejected automatically[cite: 31, 44].
* **Win Condition:** Connect 4 pieces horizontally, vertically, or diagonally[cite: 30, 44]. If all 42 cells fill up with no winner, the game ends in a draw[cite: 30, 44].

---

## How the AI Thinks

The computer evaluates candidate moves using an iterative 3-ply game tree search[cite: 30, 44]:

1. **3-Ply Look-Ahead:**
   * **Ply 1 (Computer `O`):** Evaluates its own possible moves, prioritizing center columns in the order `3, 2, 4, 1, 5, 0, 6`[cite: 30, 31, 44].
   * **Ply 2 (Human `X`):** Assumes the human will play the optimal counter-move (minimizing the computer's score)[cite: 30, 44].
   * **Ply 3 (Computer `O`):** Evaluates its best follow-up response[cite: 30, 44].
2. **Alpha-Beta Pruning (`L` and `U` Bounds):** Tracks the guaranteed lower bound (`L`) for the computer and upper bound (`U`) for the human, pruning remaining column branches whenever `L >= U`[cite: 30, 31, 44].
3. **Board Evaluation Heuristic:**
   * Immediate terminal wins are scored as `+(100000 + 3 - ply)` for the computer and `-(100000 + 3 - ply)` for the human (favoring faster wins and delaying losses)[cite: 31, 44].
   * Non-terminal positions at Ply 3 are scored across every 4-cell window (`+5` for 3 `O`s + 1 empty, `+2` for 2 `O`s + 2 empty, `-4` for 3 `X`s + 1 empty) plus a `+3` positional bonus for every computer piece in the center column (`column 3`)[cite: 31, 44].

---

## How to Compile and Run

Works on any operating system (Windows, Linux, or macOS) with a standard C++ compiler:

```bash
g++ q7.cpp -o connect4
./connect4