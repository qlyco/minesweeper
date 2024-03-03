#include <ncurses.h>

#include <cstdlib>
#include <stdio.h>
#include <map>
#include <string>
#include <time.h>

#include "cli_parser.hh"
#include "main_menu.hh"
#include "minefield.hh"
#include <game.hh>

int main(int argc, char *argv[])
{
    std::map<std::string, std::string> options = parse_args(argc, argv);

    if (options.find("SEED") != options.end())
    {
        const char *seed = options["SEED"].c_str();
        srand(strtol(seed, nullptr, 10));
    }
    else
    {
        srand(time(0));
    }

    int diff = -1;

    if (options.find("DIFFICULTY") != options.end())
    {
        const char *mode = options["DIFFICULTY"].c_str();
        diff = strtol(mode, nullptr, 10);
    }

    initscr();
    start_color();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (diff == -1)
        diff = show_main_menu();

    Minefield play_area = Minefield();

    clear();
    switch (diff)
    {
    case 0:
        mvprintw(1, 1, "Beginner level selected.");
        play_area.width = 9;
        play_area.height = 9;
        play_area.mines = 10;
        break;
    case 1:
        mvprintw(1, 1, "Intermediate level selected.");
        play_area.width = 16;
        play_area.height = 16;
        play_area.mines = 40;
        break;
    case 2:
        mvprintw(1, 1, "Expert level selected.");
        play_area.width = 30;
        play_area.height = 16;
        play_area.mines = 99;
        break;
    case 3:
        if (options.find("WIDTH") != options.end())
        {
            const char *width = options["WIDTH"].c_str();
            play_area.width = strtol(width, nullptr, 10);
        } else {
            diff = -1;
            break;
        }

        if (options.find("HEIGHT") != options.end())
        {
            const char *seed = options["HEIGHT"].c_str();
            play_area.height = strtol(seed, nullptr, 10);
        } else {
            diff = -1;
            break;
        }

        if (options.find("MINES") != options.end())
        {
            const char *seed = options["MINES"].c_str();
            play_area.mines = strtol(seed, nullptr, 10);
        } else {
            diff = -1;
            break;
        }

        if (play_area.width * play_area.height <= play_area.mines || play_area.width < 9 || play_area.height < 9 || play_area.mines <= 0) {
            diff = -1;
            break;
        }

        break;
    default:
        break;
    }

    if (diff != -1)
    {
        int result = play(&play_area);
        clear();
        endwin();

        if (result == 0) {
            printf("You lose.\n");
        } else {
            printf("You win!\n");
        }
    }
    else
    {
        endwin();
        printf("USAGE: minesweeper [OPTIONS] [VALUE]\n");
        printf("\nOptions:\n");
        printf("  --diff / -D = Set difficulty to the given VALUE. (VALUES = 0, 1, 2, 3)\n");
        printf("  --seed / -S = Set the seed to the given VALUE.\n");
        printf("  --width / -W = Set the width to the given VALUE. (Difficulty 3 only)\n");
        printf("  --height / -H = Set the height to the given VALUE. (Difficulty 3 only)\n");
        printf("  --mines / -M = Set the mines to the given VALUE. (Difficulty 3 only)\n");
    }
}