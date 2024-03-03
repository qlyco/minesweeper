#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "main_menu.hh"

int show_main_menu() {
    int selected = 0;
    int input = 0;

    std::vector<std::string> menu_items = {
        " Beginner        ",
        " Intermediate    ",
        " Expert          "
    };

    while (input != 10) {
        redraw_main_menu(&menu_items, selected);
        input = getch();

        switch(input) {
            case KEY_UP:
                --selected;
                if (selected < 0)
                    selected = menu_items.size() - 1;
                break;
            case KEY_DOWN:
                ++selected;
                if (selected >= menu_items.size())
                    selected = 0;
                break;
            default:
                break;
        }
    }

    return selected;
}

void redraw_main_menu(std::vector<std::string> *menu_items, int selected) {
    clear();
    mvprintw(1, 1, "QLYCOWorks's MINESWEEPER");
    mvprintw(3, 1, "Difficulty:");
    for (int i = 0; i < menu_items->size(); i++) {
        if (i == selected) {
            wattron(stdscr, A_STANDOUT);
            mvprintw(i + 4, 3, (*menu_items)[i].c_str());
            wattroff(stdscr, A_STANDOUT);
        } else {
            mvprintw(i + 4, 3, (*menu_items)[i].c_str());
        }
    }

    mvprintw(selected + 4, 1, ">");

    mvprintw(8, 1, "(C)2024, QLYCO / dfx.");

    wrefresh(stdscr);
}