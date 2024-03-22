all: compile run clean

build: build-linux build-win

build-win:
	@mkdir -p bin
	@x86_64-w64-mingw32-g++ -Wall -g -Wextra -Werror -pedantic -static -s -O -DTARGET_WIN32 -I./includes/ ./src/*.cc -L./libs/ -lpdcurses -o bin/minesweeper.exe

build-linux:
	@mkdir -p bin
	@g++ -Wall -g -Wextra -Werror -pedantic -s -O ./src/*.cc -lncursesw -ltinfo -o bin/minesweeper

compile:
	@mkdir -p bin
	@g++ -Wall -g -Wextra -Werror -pedantic ./src/*.cc -lncurses -o bin/minesweeper

run:
	@clear
	@bin/minesweeper --debug -S 8100

clean:
	@rm -rf bin
	@clear