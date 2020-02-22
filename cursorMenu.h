#ifndef CURSORMENU_H_INCLUDED
#define CURSORMENU_H_INCLUDED

#include <vector>
#include "SubMenu.h"

class cursorMenu {
public:
    cursorMenu(int pos_x = 0, int pos_y = 0, int width = 100, int height = 30):
        x(pos_x), y(pos_y), width(width), height(height)
    {
        font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",12,0); // load small font
    }
    void push(const char *label) {
        SubMenu *newElem = new SubMenu(label, width, height);
        menu_set.push_back(newElem);
    }
    void Draw() {
        int i=0;
        for(auto submenu: menu_set) {
            submenu->Draw(font, x, y + height*i);
            i++;
        }
    }
    void UpdateCoordinate(int newX, int newY) {
        x = newX;
        y = newY;
    }
    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return width; }
    int getHeight() { return height * menu_set.size(); }

    char* getButtonLabelByYAxis(int clicked_y) {
        int index = (int)((clicked_y - y) / height);

        return menu_set[index]->getLabel();
    }
private:
    std::vector<SubMenu*> menu_set;
    int x, y;
    int width, height;
    ALLEGRO_FONT *font = NULL;
};


#endif // CURSORMENU_H_INCLUDED
