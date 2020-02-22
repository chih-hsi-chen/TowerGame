#ifndef SUBMENU_H_INCLUDED
#define SUBMENU_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "global.h"

class SubMenu {

public:
    SubMenu(const char *newLabel, int width = 40, int height = 30):
        width(width),
        height(height)
    {
        strncpy(label, newLabel, 20);
    }
    void Draw(ALLEGRO_FONT *font, int x = 0, int y = 0) {
        al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb(255, 255, 255));
        al_draw_rectangle(x, y, x + width, y + height, al_map_rgb(0, 0, 0), 2);
        al_draw_text(font, al_map_rgb(0, 0, 0), x + 5, y + (height - 20)/2, 0, label);
    }
    char* getLabel() { return label; }

private:
    char label[20];
    int width;
    int height;
};

#endif // SUBMENU_H_INCLUDED
