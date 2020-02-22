#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <list>
#include <time.h>
#include "Menu.h"
#include "Level.h"
#include "WolfKnight.h"
#include "CaveMan.h"
#include "Wolf.h"
#include "DemonNijia.h"
#include "Arcane.h"
#include "Archer.h"
#include "Canon.h"
#include "Poison.h"
#include "Storm.h"
#include "Attack.h"
#include "Slider.h"
#include "cursorMenu.h"

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7

// clock rate
const float FPS = 60;

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

class GameWindow
{
public:
    // constructor
    GameWindow();
    // each process of scene
    void game_init();
    void game_reset();
    void game_play();

    void game_start_scene();
    void game_setting();
    void game_select_level();
    void game_begin();

    int game_run();
    int game_update();

    int game_go_next();
    int game_failed();

    void show_err_msg(int msg);
    void game_destroy();

    // each drawing scene function
    void draw_running_map();
    void draw_paused_map();
    void draw_start_scene_map();
    void draw_setting_scene_map();
    void draw_select_scene_map();
    void draw_score_board(bool success = true);

    // process of updated event
    int process_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);
    // detect if a tower will be constructed on road
    bool isOnRoad();
    // check if cursor menu should close.
    bool cursorMenu_nextState();
    //
    int handleMenuEvent(int, int);

    Tower* create_tower(int);
    Monster* create_monster();

public:
    bool initial = true;

private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *tower[Num_TowerType];
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *startScene = NULL;
    ALLEGRO_BITMAP *startButton = NULL;
    ALLEGRO_BITMAP *settingButton = NULL;
    ALLEGRO_BITMAP *exitButton = NULL;
    ALLEGRO_BITMAP *level_img[LevelNum];
    ALLEGRO_BITMAP *backArrow = NULL;

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *monster_pro = NULL;

    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *startSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *clearSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *failSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = NULL;

    // button color for score board
    ALLEGRO_COLOR actionColor;
    ALLEGRO_COLOR exitColor;

    // button colors for pause panel
    ALLEGRO_COLOR resumeColor;
    ALLEGRO_COLOR menuColor;

    LEVEL *level = NULL;
    Menu *menu = NULL;
    Slider *backgroundSlider = NULL, *effectSlider = NULL;
    cursorMenu *cursor_menu;

    std::vector<Monster*> monsterSet;
    std::list<Tower*> towerSet;

    int Monster_Pro_Count = 0;
    int Coin_Inc_Count = 0;
    int mouse_x, mouse_y;
    int selectedTower = -1, lastClicked = -1;
    int drawFrame = -1;

    bool redraw = false;
    bool pause = false;
    bool mute = false;
    bool isMenuOpened = false;
};


#endif // MAINWINDOW_H_INCLUDED
