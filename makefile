PDCURSES_PATH=/usr/local/

all: compile run clean

build-win:
	@mkdir -p bin
	@x86_64-w64-mingw32-g++ -Wall -g -Wextra -Werror -pedantic -static -s -O -DTARGET_WIN32 -I$(PDCURSES_PATH)include/ -I./libs/ ./src/*.cc ./libs/*.cc -L$(PDCURSES_PATH)lib/ -lpdcurses -o bin/minesweeper.exe

build-linux:
	@mkdir -p bin
	@g++ -Wall -g -Wextra -Werror -pedantic -static -s -O -I./libs/ ./src/*.cc ./libs/*.cc -lncurses -o bin/minesweeper

compile:
	@mkdir -p bin
	@g++ -Wall -g -Wextra -Werror -pedantic -I./libs/ ./src/*.cc ./libs/*.cc -lncurses -o bin/minesweeper

run:
	@clear
	@bin/minesweeper --debug -S 8100

clean:
	@rm bin/minesweeper
	@clear