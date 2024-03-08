#include <vector>
#include "minefield.hh"
#include <cstdlib>
#include <string>
#include <unistd.h>

void generate_minefield(Minefield *play_area, int click_x, int click_y)
{
    std::vector<int> field(play_area->width * play_area->height, ' ');

    {
        int mines_left = play_area->mines;

        while (mines_left > 0)
        {
            int x = rand() % play_area->width;
            int y = rand() % play_area->height;

            if (x == click_x && y == click_y)
                continue;

            if (field[x + y * play_area->width] == 'X')
                continue;

            field[x + y * play_area->width] = 'X';
            --mines_left;
        }
    }

    play_area->field = field;
}

void generate_hints(Minefield *play_area)
{
    int digits[] = {' ', '1', '2', '3', '4', '5', '6', '7', '8'};

    int skipped = 0;

    for (int y = 0; y < play_area->height; y++)
    {
        for (int x = 0; x < play_area->width; x++)
        {
            if (play_area->field[x + y * play_area->width] == 'X')
            {
                ++skipped;
                continue;
            }

            int mines = 0;

            for (int j = -1; j < 2; j++)
            {
                for (int i = -1; i < 2; i++)
                {
                    if (i == 0 && j == 0)
                        continue;
                    if (!in_bounds(x + i, y + j, play_area->width, play_area->height))
                        continue;

                    if (play_area->field[(x + i) + (y + j) * play_area->width] == 'X')
                        ++mines;
                }
            }

            play_area->field[x + y * play_area->width] = digits[mines];
        }
    }
}

bool in_bounds(int x, int y, int w, int h)
{
    return x >= 0 && x < w && y >= 0 && y < h;
}