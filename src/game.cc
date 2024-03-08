#ifndef TARGET_WIN32
#include <ncurses.h>
#endif

#ifdef TARGET_WIN32
#include "curses.h"
#endif

#include "minefield.hh"

#include <vector>
#include <deque>

#define BASE_CLR 0
#define HINT_CLR 1
#define MINE_CLR 2
#define FLAG_CLR 3
#define TILE_CLR 4
#define CURS_CLR 5
#define WALL_CLR 6

void _redraw(Minefield *play_area, std::vector<int> *revealed);
void _draw_cursor(int x, int y);
int _flood_fill(int x, int y, Minefield *play_area, std::vector<int> *revealed);
int _quick_reveal(int *x, int *y, Minefield *play_area, std::vector<int> *revealed);
int _count_flags(std::vector<int> *revealed);

int play(Minefield *play_area)
{
    init_pair(BASE_CLR, COLOR_WHITE, COLOR_BLACK);
    init_pair(HINT_CLR, COLOR_BLUE, COLOR_BLACK);
    init_pair(MINE_CLR, COLOR_RED, COLOR_BLACK);
    init_pair(FLAG_CLR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CURS_CLR, COLOR_GREEN, COLOR_BLACK);
    init_pair(WALL_CLR, COLOR_YELLOW, COLOR_BLACK);

    std::vector<int> revealed(play_area->width * play_area->height, -1);

    int input = 0;
    bool ended = false;

    int x = 0;
    int y = 0;

    int not_mines = 0;
    int flags = 0;

    int win = 0;

    bool first_click = true;

    while (!ended)
    {
        _redraw(play_area, &revealed);
        _draw_cursor(x, y);
        wattron(stdscr, COLOR_PAIR(WALL_CLR));
        mvprintw(play_area->height + 1, 2, "BOMBS: %-3d | FLAGS: %-3d", play_area->mines, flags);
        wattroff(stdscr, COLOR_PAIR(WALL_CLR));
        wrefresh(stdscr);

        input = getch();

        switch (input)
        {
        case 'w':
        case KEY_UP:
            --y;
            if (y < 0)
                y = play_area->height - 1;
            break;
        case 's':
        case KEY_DOWN:
            ++y;
            if (y >= play_area->height)
                y = 0;
            break;
        case 'a':
        case KEY_LEFT:
            --x;
            if (x < 0)
                x = play_area->width - 1;
            break;
        case 'd':
        case KEY_RIGHT:
            ++x;
            if (x >= play_area->width)
                x = 0;
            break;
        case 'x':
        case 'l':
        case 10: // ENTER
            if (first_click)
            {
                bool valid = false;

                do
                {
                    generate_minefield(play_area, x, y);
                    generate_hints(play_area);

                    if (play_area->field[x + y * play_area->width] == ' ')
                        valid = true;
                } while (!valid);
                first_click = false;
            }

            if (revealed[x + y * play_area->width] == 1) {
                break;
            }

            if (play_area->field[x + y * play_area->width] == 'X')
            {
                ended = true;
                std::fill(revealed.begin(), revealed.end(), 0);
                _redraw(play_area, &revealed);
                _draw_cursor(x, y);
                wattron(stdscr, COLOR_PAIR(WALL_CLR));
                mvprintw(play_area->height + 1, 2, "GAME OVER");
                wattroff(stdscr, COLOR_PAIR(WALL_CLR));

                wrefresh(stdscr);
                getch();
            }
            else if (play_area->field[x + y * play_area->width] == ' ')
            {
                not_mines += _flood_fill(x, y, play_area, &revealed);
                flags = _count_flags(&revealed);
            }
            else
            {
                if (revealed[x + y * play_area->width] != 0)
                {
                    revealed[x + y * play_area->width] = 0;
                    ++not_mines;
                }
                else
                {
                    int result = _quick_reveal(&x, &y, play_area, &revealed);

                    if (result == -1)
                    {
                        ended = true;
                        std::fill(revealed.begin(), revealed.end(), 0);
                        _redraw(play_area, &revealed);
                        _draw_cursor(x, y);
                        wattron(stdscr, COLOR_PAIR(WALL_CLR));
                        mvprintw(play_area->height + 1, 2, "GAME OVER");
                        wattroff(stdscr, COLOR_PAIR(WALL_CLR));
                        
                        wrefresh(stdscr);
                        getch();
                    } else {
                        not_mines += result;
                    }
                }
            }

            break;
        case 'z':
        case 'k':
        case KEY_BACKSPACE:
        case 32:
            if (revealed[x + y * play_area->width] == -1) {
                revealed[x + y * play_area->width] = 1;
                ++flags;
            }
            else if (revealed[x + y * play_area->width] == 1) {
                revealed[x + y * play_area->width] = -1;
                --flags;
            }
            break;
        case 'q':
            ended = true;
        default:
            break;
        }

        if (not_mines == play_area->width * play_area->height - play_area->mines)
        {
            ended = true;
            win = 1;
            std::fill(revealed.begin(), revealed.end(), 2);
            _redraw(play_area, &revealed);
            wattron(stdscr, COLOR_PAIR(WALL_CLR));
            mvprintw(play_area->height + 1, 2, "YOU WIN!");
            wattroff(stdscr, COLOR_PAIR(WALL_CLR));

            wrefresh(stdscr);
            getch();
        }
    }

    return win;
}

void _redraw(Minefield *play_area, std::vector<int> *revealed)
{
    clear();

    for (int i = 0; i < play_area->width * 3 + 2; i++) {
        attron(COLOR_PAIR(WALL_CLR));
        mvaddch(0, i, ACS_HLINE);
        attroff(COLOR_PAIR(WALL_CLR));
    }

    for (int y = 0; y < play_area->height; y++)
    {
        attron(COLOR_PAIR(WALL_CLR));
        mvaddch(y + 1, 0, ACS_VLINE);
        attroff(COLOR_PAIR(WALL_CLR));

        for (int x = 0; x < play_area->width; x++)
        {
            char icon = ' ';
            int cur_clr = BASE_CLR;

            switch ((*revealed).at(x + y * play_area->width))
            {
            case 0:
                icon = play_area->field[x + y * play_area->width];
                cur_clr = HINT_CLR;

                if (icon == 'X')
                {
                    cur_clr = MINE_CLR;
                }

                break;
            case -1:
                icon = '.';
                cur_clr = BASE_CLR;
                break;
            case 1:
                icon = 'P';
                cur_clr = FLAG_CLR;
                break;
            case 2:
                icon = play_area->field[x + y * play_area->width];
                if (icon == 'X')
                {
                    icon = 'P';
                    cur_clr = FLAG_CLR;
                }
                else
                {
                    cur_clr = HINT_CLR;
                }
            }

            attron(COLOR_PAIR(cur_clr));
            mvaddch(y + 1, x * 3 + 2, icon);
            attroff(COLOR_PAIR(cur_clr));
        }

        attron(COLOR_PAIR(WALL_CLR));
        mvaddch(y + 1, play_area->width * 3 + 1, ACS_VLINE);
        attroff(COLOR_PAIR(WALL_CLR));
    }

    for (int i = 0; i < play_area->width * 3 + 2; i++) {
        attron(COLOR_PAIR(WALL_CLR));
        mvaddch(play_area->height + 1, i, ACS_HLINE);
        attroff(COLOR_PAIR(WALL_CLR));
    }

    attron(COLOR_PAIR(WALL_CLR));
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, play_area->width * 3 + 1, ACS_URCORNER);
    mvaddch(play_area->height + 1, 0, ACS_LLCORNER);
    mvaddch(play_area->height + 1, play_area->width * 3 + 1, ACS_LRCORNER);
    attroff(COLOR_PAIR(WALL_CLR));
}

void _draw_cursor(int x, int y)
{
    attron(COLOR_PAIR(CURS_CLR));
    mvaddch(y + 1, x * 3 + 1, '[');
    mvaddch(y + 1, x * 3 + 3, ']');
    attroff(COLOR_PAIR(CURS_CLR));
}

int _flood_fill(int start_x, int start_y, Minefield *play_area, std::vector<int> *revealed)
{
    std::deque<std::pair<int, int>> tiles;
    tiles.push_back(std::pair<int, int>(start_x, start_y));

    int count = 0;

    while (tiles.size() > 0)
    {
        int cur_x = tiles.front().first;
        int cur_y = tiles.front().second;

        tiles.pop_front();

        if ((*revealed)[cur_x + cur_y * play_area->width] == 0)
            continue;

        if (play_area->field[cur_x + cur_y * play_area->width] != ' ')
        {
            (*revealed)[cur_x + cur_y * play_area->width] = 0;
            ++count;
        }
        else if (play_area->field[cur_x + cur_y * play_area->width] == ' ')
        {
            (*revealed)[cur_x + cur_y * play_area->width] = 0;
            ++count;

            if (cur_x - 1 >= 0 && (*revealed)[(cur_x - 1) + cur_y * play_area->width] != 0)
                tiles.push_back({cur_x - 1, cur_y});
            if (cur_x + 1 < play_area->width && (*revealed)[(cur_x + 1) + cur_y * play_area->width] != 0)
                tiles.push_back({cur_x + 1, cur_y});
            if (cur_y - 1 >= 0 && (*revealed)[cur_x + (cur_y - 1) * play_area->width] != 0)
                tiles.push_back({cur_x, cur_y - 1});
            if (cur_y + 1 < play_area->height && (*revealed)[cur_x + (cur_y + 1) * play_area->width] != 0)
                tiles.push_back({cur_x, cur_y + 1});
        }
    }

    return count;
}

int _quick_reveal(int *x, int *y, Minefield *play_area, std::vector<int> *revealed)
{
    int flags = 0;
    int hint = play_area->field[*x + *y * play_area->width] - '0';

    for (int j = -1; j < 2; j++)
    {
        for (int i = -1; i < 2; i++)
        {
            if (i == 0 && j == 0)
                continue;
            if (!in_bounds(*x + i, *y + j, play_area->width, play_area->height))
                continue;

            if ((*revealed)[(*x + i) + (*y + j) * play_area->width] == 1)
                ++flags;
        }
    }

    if (flags < hint)
        return 0;

    int shown = 0;

    for (int j = -1; j < 2; j++)
    {
        for (int i = -1; i < 2; i++)
        {
            if (i == 0 && j == 0)
                continue;
            if (!in_bounds(*x + i, *y + j, play_area->width, play_area->height))
                continue;

            if ((*revealed)[(*x + i) + (*y + j) * play_area->width] == 1)
                continue;
            else if ((*revealed)[(*x + i) + (*y + j) * play_area->width] == -1)
            {
                if (play_area->field[(*x + i) + (*y + j) * play_area->width] == 'X')
                {
                    *x += i;
                    *y += j;
                    return -1;
                }
                else if (play_area->field[(*x + i) + (*y + j) * play_area->width] != ' ')
                {
                    (*revealed)[(*x + i) + (*y + j) * play_area->width] = 0;
                    ++shown;
                }
                else
                {
                    shown += _flood_fill(*x + i, *y + j, play_area, revealed);
                }
            }
        }
    }

    return shown;
}

int _count_flags(std::vector<int> *revealed) {
    int flags = 0;

    for (int i : *revealed) {
        if (i == 1) {
            ++flags;
        }
    }

    return flags;
}