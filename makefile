all: compile run clean

build:
	g++ ./src/*.cc ./libs/*.cc -lncurses -I./libs/ -s -o bin/minesweeper

compile:
	g++ ./src/*.cc ./libs/*.cc -lncurses -I./libs/ -o bin/minesweeper

run:
	clear
	bin/minesweeper --debug -S 8100

clean:
	rm bin/minesweeper
	clear