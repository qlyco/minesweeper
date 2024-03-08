# mineweeper

A minesweeper clone written in C/C++ to be run in a terminal with ```ncurses``` and ```pdcurses``` support.
Windows build was cross-compiled from Linux.

## Controls

- WASD / Arrow keys = Move cursor
- Space = Flag a cell
- Enter = Reveal a cell

## Features

- [X] Three different difficulties + hidden custom difficulty
- [X] Safe first cell, first cell always guaranteed to be a zero cell
- [X] Colour support
- [X] Quick reveal (press enter on a numbered cell once enough flags were placed around it to reveal unflagged cells)
- [X] Windows & Linux support

## Planned Features

- [ ] GUI support
- [ ] Leaderboard / score system
- [ ] No guesses mode
- [ ] Refactored / cleaner codebase (current code is too hacky and written in a few hours)

## Known Issues

- Larger field sizes will be slower to be redrawn
- When playing custom difficulty, the game will be softlocked after the first click if the number of safe spaces is too little compared to the available cells (eg. 10x10 board with 99 mines)
- Flag count wasn't updated properly when flags was removed after clicking on a zero cell

## Custom Difficulty

Run the app with the following commandline arguments: ```minesweeper -D 3 -W [width] -H [height] -M [mines]```

- ```width``` and ```height``` must be at least 9
- ```mines``` must be at least 1

## Build Instructions

### Linux

Linux build is easy to compile. We recommend WSL if you're on Windows:
- Make sure ```ncurses``` is installed.
- Make sure ```g++``` is installed.
- Run ```make build```.
- Run the built executable in ```bin```.

### Windows

Windows build need to be cross-compiled from Linux and is more complicated. WSL is recommended:
- Download and build ```pdcurses```.
- Place ```libpdcurses.a``` and all related headers in a common directory, separating them into ```lib``` and ```include``` sub-directory.
- Make sure ```mingw-w64``` is installed.
- Run ```make build-win PDCURSES_PATH=[path]```, replacing the ```[path]``` with the directory path where you stored the built ```pdcurses```.
- Run the built executable in ```bin```.