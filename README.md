🧨 Minesweeper (C++ with SFML)
A classic Minesweeper game implemented in C++, featuring a graphical user interface built with SFML and a persistent leaderboard system using local file I/O.

🚀 Features
Classic Minesweeper gameplay (click to reveal, right-click to flag, etc.)

Pause button stops the timer and blocks off game screen

Debug button shows where all mines currently are

Customizable board size and mine count

Timer and mine counter display

Local leaderboard: saves and displays fastest completion times

Graceful win/loss detection with UI feedback

🖼️ Screenshots

<img width="596" height="429" alt="Screenshot 2025-07-23 at 1 10 19 PM" src="https://github.com/user-attachments/assets/61acd66e-475e-4db0-888c-9d63b35ce80a" />
<img width="596" height="429" alt="Screenshot 2025-07-23 at 1 10 51 PM" src="https://github.com/user-attachments/assets/665f5054-0a36-458b-baef-9b557784b1e5" />
<img width="596" height="429" alt="Screenshot 2025-07-23 at 1 12 07 PM" src="https://github.com/user-attachments/assets/92304726-4681-4456-8bc9-6e8934e2174d" />


🛠️ Tech Stack
C++ – Core game logic and file I/O

SFML (Simple and Fast Multimedia Library) – UI, rendering, and input handling

Standard Library (fstream, vector, etc.) – Game state and leaderboard management

📈 Leaderboard System
Stores fastest game completion times locally in leaderboard.txt

Reads and writes using simple file I/O

Displays top scores in-game

🎮 Controls
Action	Control
Reveal cell	Left-click
Flag cell	Right-click
Restart game	Click "Restart" button (if implemented)

🔧 Setup Instructions
Install SFML
You’ll need SFML installed on your system. Follow instructions from: https://www.sfml-dev.org/download.php

Build the project

bash
Copy
Edit
g++ -std=c++17 -o minesweeper src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system
Run the game

bash
Copy
Edit
./minesweeper

