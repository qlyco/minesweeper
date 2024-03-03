#ifndef MINEFIELD_HH
#define MINEFIELD_HH

#include <vector>

struct Minefield {
    int width   = 9;
    int height  = 9;
    int mines   = 10;
    std::vector<int> field;
};

void build_map(Minefield *play_area);
void generate_minefield(Minefield *play_area, int x, int y);
void generate_hints(Minefield *play_area);
bool in_bounds(int x, int y, int w, int h);

#endif