#ifndef MAIN_MENU_HH
#define MAIN_MENU_HH

#include <vector>
#include <string>

int show_main_menu();
void redraw_main_menu(std::vector<std::string> *menu_items, int selected);

#endif