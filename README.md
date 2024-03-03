# mineweeper

A minesweeper clone written in C/C++ to be run in a terminal with ```ncurses``` support.
No support for Windows yet.

## Controls

- WASD / Arrow keys = Move cursor
- Space = Flag a cell
- Enter = Reveal a cell

## Features

- [X] : Three different difficulties + hidden custom difficulty
- [X] : Safe first cell, first cell always guaranteed to be a zero cell
- [X] : Colour support
- [X] : Quick reveal (press enter on a numbered cell once enough flags were placed around it to reveal unflagged cells)

## Planned Features

- [ ] : Windows support
- [ ] : GUI support
- [ ] : Leaderboard / score system
- [ ] : No guesses mode
- [ ] : Refactored / cleaner codebase (current code is too hacky and written in a few hours)

## Known Issues

- Larger field sizes will be slower to be redrawn
- When playing custom difficulty, the game will be softlocked after the first click if the number of safe spaces is too little compared to the available cells (eg. 10x10 board with 99 mines)

## Custom Difficulty

Run the app with the following commandline arguments: ```minesweeper -D 3 -W [width] -H [height] -M [mines]```

- ```width``` and ```height``` must be at least 9
- ```mines``` must be at least 1

## Build Instructions

- Make sure ```ncurses``` is installed.
- Make sure you can compile C/C++ programs.
- Run ```make build```.
- Run the built executable in ```bin```.
