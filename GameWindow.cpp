#include "GameWindow.h"
#include "global.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))


float Attack::volume = 1.0;

void set_attack_volume(float volume)
{
    Attack::volume = volume;
}

bool compare(Tower *t1, Tower *t2)
{
    return (t1->getY() <= t2->getY());
}

bool
GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if(mouse_x >= startx && mouse_x <= startx + width)
        if(mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

bool
GameWindow::cursorMenu_nextState() {
    int x = cursor_menu->getX();
    int y = cursor_menu->getY();
    int width = cursor_menu->getWidth();
    int height = cursor_menu->getHeight();

    return mouse_hover(x, y, width, height);
}

int
GameWindow::handleMenuEvent(int x, int y) {
    const char* label = cursor_menu->getButtonLabelByYAxis(y);

    if(strcmp(label, "Exit") == 0)
        return GAME_EXIT;
    else if(strcmp(label, "Menu") == 0)
        return GAME_TERMINATE;
}

void
GameWindow::game_init()
{
    char buffer[50];

    icon = al_load_bitmap("./icon.png");
    background = al_load_bitmap("./StartBackground.jpg");

    startScene = al_load_bitmap("./StartScene/Background.png");
    startButton = al_load_bitmap("./StartScene/Start.png");
    settingButton = al_load_bitmap("./StartScene/Gear.png");
    exitButton = al_load_bitmap("./StartScene/Exit.png");

    backArrow = al_load_bitmap("./SettingScene/BackArrow.png");

    if
    (
       icon == NULL ||
       background == NULL ||
       startScene == NULL ||
       startButton == NULL ||
       settingButton == NULL ||
       exitButton == NULL ||
       backArrow == NULL
    )
    {
        initial = false;
    }

    for(int i = 0; i < Num_TowerType; i++)
    {
        sprintf(buffer, "./Tower/%s.png", TowerClass[i]);
        tower[i] = al_load_bitmap(buffer);

        if(tower[i] == NULL)
            initial = false;
    }

    for(int i=0; i < LevelNum; i++)
    {
        sprintf(buffer, "./SelectScene/level%d.jpg", i+1);
        level_img[i] = al_load_bitmap(buffer);

        if(level_img[i] == NULL)
            initial = false;
    }

    al_set_display_icon(display, icon);
    al_reserve_samples(5);

    sample = al_load_sample("growl.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());

    sample = al_load_sample("level_clear.wav");
    clearSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(clearSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(clearSound, al_get_default_mixer());

    sample = al_load_sample("level_fail.wav");
    failSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(failSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(failSound, al_get_default_mixer());

    sample = al_load_sample("BackgroundMusic.ogg");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());

    level = new LEVEL(1);
    menu = new Menu();
    backgroundSlider = new Slider(window_width/2 - 100, 200);
    effectSlider = new Slider(window_width/2 - 100, 350);
    cursor_menu = new cursorMenu();
    cursor_menu->push("Menu");
    cursor_menu->push("Exit");

    backgroundSlider->set_label_content("BackGround Sound");
    effectSlider->set_label_content("Effect Sound");

    actionColor = WHITE;
    exitColor = WHITE;
    resumeColor = WHITE;
    menuColor = WHITE;


}

bool
GameWindow::isOnRoad()
{
    int startx, starty;
    int widthOfTower;

    widthOfTower = TowerWidth[selectedTower];

    for(int i=0; i < NumOfGrid; i++)
    {
        startx = (i % 15) * 40;
        starty = (i / 15) * 40;

        if(level->isRoad(i)) {
            if((mouse_x + (widthOfTower/2) < startx) || (mouse_x - (widthOfTower/2) > startx + grid_width))
                continue;
            else if((mouse_y + (widthOfTower/2) < starty) || (mouse_y > starty + grid_height))
                continue;
            else
                return true;
        }
    }
    return false;
}

Tower*
GameWindow::create_tower(int type)
{
    Tower *t = NULL;

    if(isOnRoad())
        return t;

    switch(type)
    {
    case ARCANE:
        t = new Arcane(mouse_x, mouse_y);
        break;
    case ARCHER:
        t = new Archer(mouse_x, mouse_y);
        break;
    case CANON:
        t = new Canon(mouse_x, mouse_y);
        break;
    case POISON:
        t = new Poison(mouse_x, mouse_y);
        break;
    case STORM:
        t = new Storm(mouse_x, mouse_y);
        break;
    default:
        break;
    }

    menu->Change_Coin(menu->getTowerCoin(type));

    return t;
}

Monster*
GameWindow::create_monster()
{
    Monster *m = NULL;

    if(level->MonsterNum[WOLF])
    {
        level->MonsterNum[WOLF]--;
        m = new Wolf(level->ReturnPath());
    }
    else if(level->MonsterNum[WOLFKNIGHT])
    {
        level->MonsterNum[WOLFKNIGHT]--;
        m = new WolfKnight(level->ReturnPath());
    }
    else if(level->MonsterNum[DEMONNIJIA])
    {
        level->MonsterNum[DEMONNIJIA]--;
        m = new DemonNijia(level->ReturnPath());
    }
    else if(level->MonsterNum[CAVEMAN])
    {
        level->MonsterNum[CAVEMAN]--;
        m = new CaveMan(level->ReturnPath());
    }
    else
    {
        al_stop_timer(monster_pro);
    }

    return m;
}

void
GameWindow::game_play()
{
    int msg;

    srand(time(NULL));

    while(msg != GAME_EXIT)
    {
        msg = -1;
        game_reset();
        game_start_scene();
        game_select_level();
        game_begin();

        while (msg != GAME_TERMINATE && msg != GAME_EXIT) {
            msg = game_run();

            // If game ends at winning status or failed status
            if(msg != GAME_CONTINUE)
            {
                al_stop_sample_instance(backgroundSound);

                if(msg == GAME_NEXT_LEVEL)
                {
                    al_play_sample_instance(clearSound);
                    while(al_get_sample_instance_playing(clearSound));
                    msg = game_go_next();
                }
                else if(msg == GAME_FAIL)
                {
                    al_play_sample_instance(failSound);
                    while(al_get_sample_instance_playing(failSound));
                    msg = game_failed();
                }
            }
        }
    }

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}

GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    timer = al_create_timer(1.0 / FPS);
    monster_pro = al_create_timer(1.0 / FPS);

    if(timer == NULL || monster_pro == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",12,0); // load small font
    Medium_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",24,0); //load medium font
    Large_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",36,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(monster_pro));

    game_init();
}

void
GameWindow::game_start_scene()
{
    int status = GAME_INIT;

    printf("Loading Game Start Scene...\n");

    if(startScene == NULL || startButton == NULL || exitButton == NULL || settingButton == NULL)
        show_err_msg(-1);

    /*while(true)
    {
        if (!al_is_event_queue_empty(event_queue)) {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                show_err_msg(GAME_TERMINATE);
            else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

                if(mouse_hover(341, 350, 118, 36))
                    // do something
                else if(mouse_hover(317, 420, 165, 48))
                    // do something
                else if(mouse_hover(10, 10, 100, 100))
                    // do something
            }
        }
    }*/
    while(status != GAME_SELECT)
    {
        if (!al_is_event_queue_empty(event_queue)) {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                show_err_msg(GAME_TERMINATE);
            else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

                if(mouse_hover(341, 350, 118, 36))
                    status = GAME_SELECT;
                else if(mouse_hover(317, 420, 165, 48))
                    show_err_msg(GAME_TERMINATE);
                else if(mouse_hover(10, 10, 100, 100))
                    status = GAME_SETTING;
            }
        }

        if(status == GAME_INIT)
        {
            al_destroy_bitmap(startButton);
            al_destroy_bitmap(exitButton);
            al_destroy_bitmap(settingButton);

            if(mouse_hover(341, 350, 118, 36))
                startButton = al_load_bitmap("./StartScene/Start_Mask.png");
            else
                startButton = al_load_bitmap("./StartScene/Start.png");

            if(mouse_hover(346, 420, 107, 37))
                exitButton = al_load_bitmap("./StartScene/Exit_Mask.png");
            else
                exitButton = al_load_bitmap("./StartScene/Exit.png");

            if(mouse_hover(10, 10, 100, 100))
                settingButton = al_load_bitmap("./StartScene/Gear_Mask.png");
            else
                settingButton = al_load_bitmap("./StartScene/Gear.png");

            draw_start_scene_map();
        }
        else if(status == GAME_SETTING)
        {
            game_setting();
            status = GAME_INIT;
        }

    }
}

void
GameWindow::game_setting()
{
    if(backArrow == NULL)
        show_err_msg(-1);

    while(true)
    {
        if(!al_event_queue_is_empty(event_queue))
        {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                show_err_msg(GAME_TERMINATE);
            else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;

                if(backgroundSlider->isDragged())
                    backgroundSlider->Drag(mouse_x, mouse_y);
                else if(effectSlider->isDragged())
                    effectSlider->Drag(mouse_x, mouse_y);
                else
                {
                    al_destroy_bitmap(backArrow);

                    if(mouse_hover(10, 10, 50, 38))
                        backArrow = al_load_bitmap("./SettingScene/BackArrow_Mask.png");
                    else
                        backArrow = al_load_bitmap("./SettingScene/BackArrow.png");
                }
            }
            else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(event.mouse.button == 1)
                {
                    // add your code
                    if(mouse_hover(10, 10, 50, 38))
                    {
                        al_set_sample_instance_gain(backgroundSound, backgroundSlider->getDegree());
                        al_set_sample_instance_gain(startSound, effectSlider->getDegree());
                        al_set_sample_instance_gain(clearSound, effectSlider->getDegree());
                        set_attack_volume(effectSlider->getDegree());
                        break;
                    }
                    else if(backgroundSlider->isClicked(mouse_x, mouse_y))
                        backgroundSlider->toggleDrag();
                    else if(effectSlider->isClicked(mouse_x, mouse_y))
                        effectSlider->toggleDrag();
                }
            }
            else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if(event.mouse.button == 1)
                {
                    if(backgroundSlider->isDragged())
                        backgroundSlider->toggleDrag();
                    else if(effectSlider->isDragged())
                        effectSlider->toggleDrag();
                }
            }
        }

        draw_setting_scene_map();
    }
}

void
GameWindow::game_select_level()
{
    int offsetX = 100, offsetY = 20;
    int gapX = 200, gapY = 80;
    int pictureLength = 200;

    while(true)
    {
        if(!al_event_queue_is_empty(event_queue))
        {
            drawFrame = -1;
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                show_err_msg(GAME_TERMINATE);
            else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1)
            {
                for(int i=0; i<LevelNum; i++)
                {
                    int startx = offsetX + (i % 2) * (pictureLength + gapX);
                    int starty = offsetY + (i / 2) * (pictureLength + gapY);

                    // if mouse clicks drawFrame
                    // enter the level immediately
                    // hint: mouse_hover(...)
                    if(mouse_hover(startx, starty, pictureLength, pictureLength))
                    {
                        level->setLevel(i+1);
                        return;
                    }
                }
            }
            else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;

                for(int i=0; i<LevelNum; i++)
                {
                    int startx = offsetX + (i % 2) * (pictureLength + gapX);
                    int starty = offsetY + (i / 2) * (pictureLength + gapY);

                    // if mouse hovers over any frame
                    // assign drawFrame to the index of level image
                    // hint: mouse_hover(...)
                    if(mouse_hover(startx, starty, pictureLength, pictureLength))
                    {
                        drawFrame = i;
                        break;
                    }
                }
            }
        }

        draw_select_scene_map();
    }
}

void
GameWindow::game_begin()
{
    printf(">>> Start Level[%d]\n", level->getLevel());
    draw_running_map();

    al_play_sample_instance(startSound);
    while(al_get_sample_instance_playing(startSound));
    al_play_sample_instance(backgroundSound);

    al_start_timer(timer);
    al_start_timer(monster_pro);
}

int
GameWindow::game_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
GameWindow::game_update()
{
    unsigned int i, j;
    std::list<Tower*>::iterator it;

    // check if monster enters the range of a tower
    /* your code */
    for(it = towerSet.begin(); it != towerSet.end(); it++)
    {
        for(j=0; j<monsterSet.size(); j++)
        {
            if((*it)->DetectAttack(monsterSet[j]))
                break;
        }
    }

    // update every monster
    // check if it is destroyed or reaches end point
    for(i=0; i < monsterSet.size(); i++)
    {
        bool isDestroyed = false, isReachEnd = false;

        for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++)
        {
            isDestroyed = (*it)->TriggerAttack(monsterSet[i]);

            if(isDestroyed)
                break;
        }

        isReachEnd = monsterSet[i]->Move();

        if(isDestroyed)
        {
            Monster *m = monsterSet[i];

            menu->Change_Coin(m->getWorth());
            menu->Gain_Score(m->getScore());
            monsterSet.erase(monsterSet.begin() + i);
            i--;
            delete m;

        }
        else if(isReachEnd)
        {
            Monster *m = monsterSet[i];

            if(menu->Subtract_HP())
                return GAME_FAIL;

            monsterSet.erase(monsterSet.begin() + i);
            i--;
            delete m;
        }
    }

    // update every attack set state of each created tower
    for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++)
        (*it)->UpdateAttack();


    return GAME_CONTINUE;
}

int
GameWindow::game_go_next()
{
    bool NextButtonClicked = false;

    while(!NextButtonClicked)
    {

        if (!al_is_event_queue_empty(event_queue)) {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;

                if(mouse_hover(window_width/2 - 150, 400, 300, 50))
                    actionColor = al_map_rgb(0, 0, 0);
                else
                    actionColor = al_map_rgb(255, 255, 255);

                if(mouse_hover(window_width/2 - 150, 500, 300, 50))
                    exitColor = al_map_rgb(0, 0, 0);
                else
                    exitColor = al_map_rgb(255, 255, 255);
            }
            else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1)
            {
                if(mouse_hover(window_width/2 - 150, 400, 300, 50))
                    NextButtonClicked = true;
                else if(mouse_hover(window_width/2 - 150, 500, 300, 50))
                    return GAME_TERMINATE;
            }
        }

        draw_score_board();

    }
    // go to next level
    level->setLevel(level->getLevel() + 1);

    // reset game and begin
    game_reset();
    game_begin();

    return GAME_CONTINUE;
}

int
GameWindow::game_failed()
{
    bool RestartButtonClicked = false;

    while(!RestartButtonClicked)
    {
        if(al_event_queue_is_empty(event_queue))
            continue;

        al_wait_for_event(event_queue, &event);

        if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;

            if(mouse_hover(window_width/2 - 150, 400, 300, 50))
                actionColor = al_map_rgb(0, 0, 0);
            else
                actionColor = al_map_rgb(255, 255, 255);

            if(mouse_hover(window_width/2 - 150, 500, 300, 50))
                exitColor = al_map_rgb(0, 0, 0);
            else
                exitColor = al_map_rgb(255, 255, 255);
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1)
        {
            if(mouse_hover(window_width/2 - 150, 400, 300, 50))
                RestartButtonClicked = true;
            else if(mouse_hover(window_width/2 - 150, 500, 300, 50))
                return GAME_TERMINATE;
        }

        draw_score_board(false);
    }

    // restart original level
    level->setLevel(level->getLevel());

    // reset game and begin
    game_reset();
    game_begin();

    return GAME_CONTINUE;
}

void
GameWindow::game_reset()
{
    // reset game and begin
    for(auto&& child : towerSet) {
        delete child;
    }
    towerSet.clear();
    monsterSet.clear();


    selectedTower = -1;
    lastClicked = -1;
    Coin_Inc_Count = 0;
    Monster_Pro_Count = 0;
    pause = false;
    mute = false;
    redraw = false;
    isMenuOpened = false;
    menu->Reset();

    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);
    al_stop_sample_instance(clearSound);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(monster_pro);
}

void
GameWindow::game_destroy()
{
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);
    al_destroy_timer(monster_pro);

    for(int i=0;i<5; i++)
        al_destroy_bitmap(tower[i]);

    for(int i=0;i<LevelNum;i++)
        al_destroy_bitmap(level_img[i]);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);
    al_destroy_bitmap(startScene);
    al_destroy_bitmap(startButton);
    al_destroy_bitmap(settingButton);
    al_destroy_bitmap(exitButton);
    al_destroy_bitmap(backArrow);

    al_destroy_sample(sample);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(clearSound);
    al_destroy_sample_instance(failSound);
    al_destroy_sample_instance(backgroundSound);

    //al_destroy_native_file_dialog(filechooser);

    game_reset();

    delete level;
    delete menu;
    delete backgroundSlider;
    delete effectSlider;
}

int
GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    // offset for pause window
    int offsetX = field_width/2 - 200;
    int offsetY = field_height/2 - 200;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer) {
            redraw = true;

            if(!pause)
            {
                if(Coin_Inc_Count == 0)
                    menu->Change_Coin(Coin_Time_Gain);

                Coin_Inc_Count = (Coin_Inc_Count + 1) % CoinSpeed;

                if(monsterSet.size() == 0 && !al_get_timer_started(monster_pro))
                {
                    al_stop_timer(timer);
                    return GAME_NEXT_LEVEL;
                }
            }
        }
        else {
            if(Monster_Pro_Count == 0) {
                Monster *m = create_monster();

                if(m != NULL)
                    monsterSet.push_back(m);
            }
            Monster_Pro_Count = (Monster_Pro_Count + 1) % level->getMonsterSpeed();
        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            // reserved
            // add your codes here
            case ALLEGRO_KEY_P:
                pause = !pause;
                if(pause)
                    al_stop_timer(monster_pro);
                else
                    al_start_timer(monster_pro);
                break;
            case ALLEGRO_KEY_M:
                mute = !mute;
                if(mute)
                    al_stop_sample_instance(backgroundSound);
                else
                    al_play_sample_instance(backgroundSound);
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.mouse.button == 1) {
            if(!pause)
            {
                if(selectedTower != -1 && mouse_hover(0, 0, field_width, field_height)) {
                    Tower *t = create_tower(selectedTower);

                    if(t == NULL)
                        printf("Wrong place\n");
                    else {
                        towerSet.push_back(t);
                        towerSet.sort(compare);
                    }
                }
                else if(selectedTower == -1) {
                    std::list<Tower*>::iterator it = towerSet.begin();
                    if(cursorMenu_nextState() && isMenuOpened) {
                        return handleMenuEvent(event.mouse.x, event.mouse.y);
                    }
                    else {
                        if(lastClicked != -1)
                        {
                            std::advance(it, lastClicked);
                            (*it)->ToggleClicked();
                        }
                        for(i=0, it = towerSet.begin(); it != towerSet.end(); it++, i++)
                        {
                            Circle *circle = (*it)->getCircle();
                            int t_width = (*it)->getWidth();

                            if(mouse_hover(circle->x - t_width/2, circle->y, t_width, t_width/2))
                            {
                                (*it)->ToggleClicked();
                                lastClicked = i;
                                break;
                            } else {
                                lastClicked = -1;
                            }
                        }
                    }
                }
                // check if user wants to create some kind of tower
                // if so, show tower image attached to cursor
                selectedTower = menu->MouseIn(mouse_x, mouse_y);
                isMenuOpened = isMenuOpened && cursorMenu_nextState();
            }
            else // If game status is PAUSED, process mouse click event on pause panel
            {
                if(mouse_hover(offsetX + 100, offsetY + 200, 200, 50)) // Click "RESUME"
                {
                    // add your code
                    pause = false;
                    resumeColor = WHITE;
                    al_start_timer(monster_pro);
                }
                else if(mouse_hover(offsetX + 100, offsetY + 300, 200, 50)) // Click "Menu"
                {
                    // add your code
                    pause = false;
                    menuColor = WHITE;
                    return GAME_TERMINATE;
                }
            }
        }
        else if(event.mouse.button == 2)
        {
            isMenuOpened = true;
            cursor_menu->UpdateCoordinate(event.mouse.x, event.mouse.y);
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;


        if(pause)
        {
            if(mouse_hover(offsetX + 100, offsetY + 200, 200, 50))
                resumeColor = BLACK;
            else
                resumeColor = WHITE;

            if(mouse_hover(offsetX + 100, offsetY + 300, 200, 50))
                menuColor = BLACK;
            else
                menuColor = WHITE;
        }
        else
        {
            // if mouse hovers over any tower thumb
            // draw frame on the thumb
            // hint: Menu
            /* your code */
            menu->MouseIn(mouse_x, mouse_y);
        }
    }

    if(redraw) {
        // update each object in game
        if(!pause)
            instruction = game_update();

        // Re-draw map
        draw_running_map();
        redraw = false;
    }

    return instruction;
}

void
GameWindow::draw_running_map()
{
    unsigned int i, j;

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background, 0, 0, 0);

    for(i = 0; i < field_height/40; i++)
    {
        for(j = 0; j < field_width/40; j++)
        {
            char buffer[50];
            sprintf(buffer, "%d", i*15 + j);
            if(level->isRoad(i*15 + j)) {
                al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, al_map_rgb(255, 244, 173));
            }
            //al_draw_text(font, al_map_rgb(0, 0, 0), j*40, i*40, ALLEGRO_ALIGN_CENTER, buffer);
        }
    }
    for(i=0; i<monsterSet.size(); i++)
    {
        monsterSet[i]->Draw();
    }


    for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++)
        (*it)->Draw();

    if(selectedTower != -1)
        Tower::SelectedTower(mouse_x, mouse_y, selectedTower);

    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(100, 100, 100));

    menu->Draw();

    if(pause){
        draw_paused_map();
    }
    else if(isMenuOpened) {
        cursor_menu->Draw();
    }

    al_flip_display();
}

void
GameWindow::draw_paused_map()
{
    int offsetX = field_width/2 -200;
    int offsetY = field_height/2 -200;

    al_draw_filled_rectangle(offsetX, offsetY, offsetX + 400, offsetY + 400, al_map_rgb(221, 186, 152));

    al_draw_text(Large_font, al_map_rgb(255, 255, 255), offsetX + 200, offsetY + 100, ALLEGRO_ALIGN_CENTER, "PAUSED");

    al_draw_filled_rounded_rectangle(offsetX + 100, offsetY + 200, offsetX + 300, offsetY + 250, 5, 5, al_map_rgb(100, 100, 100));
    al_draw_text(Medium_font, resumeColor, offsetX + 200, offsetY + 200 + font_size, ALLEGRO_ALIGN_CENTER, "RESUME");

    al_draw_filled_rounded_rectangle(offsetX + 100, offsetY + 300, offsetX + 300, offsetY + 350, 5, 5, al_map_rgb(100, 100, 100));
    al_draw_text(Medium_font, menuColor, offsetX + 200, offsetY + 300 + font_size, ALLEGRO_ALIGN_CENTER, "MENU");
}

void
GameWindow::draw_start_scene_map()
{
    al_clear_to_color(al_map_rgb(255,255,255));

    al_draw_bitmap(startScene, 0, 0, 0);
    al_draw_bitmap(startButton, 341, 350, 0);
    al_draw_bitmap(exitButton, 346, 420, 0);
    al_draw_bitmap(settingButton, 10, 10, 0);

    al_flip_display();
}

void
GameWindow::draw_setting_scene_map()
{
    al_clear_to_color(al_map_rgb(255, 196, 87));

    al_draw_bitmap(backArrow, 10, 10, 0);

    backgroundSlider->Draw();
    effectSlider->Draw();

    al_flip_display();
}

void
GameWindow::draw_select_scene_map()
{
    int offsetX = 100, offsetY = 20;
    int gapX = 200, gapY = 80;
    int pictureLength = 200;
    int startx, starty;

    al_clear_to_color(al_map_rgb(255, 196, 87));

    for(int i=0; i < LevelNum; i++)
    {
        startx = offsetX + (i % 2) * (pictureLength + gapX);
        starty = offsetY + (i / 2) * (pictureLength + gapY);

        al_draw_bitmap(level_img[i], offsetX + (i % 2) * (pictureLength + gapX), offsetY + (i / 2) * (pictureLength + gapY), 0);

        // if mouse hovers over a level picture, then draw a white frame around it
        if(i == drawFrame)
            al_draw_rectangle(startx, starty, startx + pictureLength, starty + pictureLength, al_map_rgb(255, 251, 189), 5);
    }
    al_flip_display();
}

void
GameWindow::draw_score_board(bool success)
{
    char levelInfo[50];
    char buttonInfo[50];
    ALLEGRO_COLOR ButtonColor;
    ALLEGRO_COLOR BackgroundColor;

    // determine the status of score board
    // if it is true, then represent win
    // otherwise, stand for lose
    if(success)
    {
        sprintf(buttonInfo, "Next Level");
        sprintf(levelInfo, "WINNING");
        ButtonColor = ORANGE_DARK;
        BackgroundColor = ORANGE_LIGHT;
    }
    else
    {
        sprintf(buttonInfo, "Restart");
        sprintf(levelInfo, "FAILED");
        ButtonColor = BLUE;
        BackgroundColor = PURPLE;
    }

    al_clear_to_color(BackgroundColor);
    al_draw_text(Large_font, al_map_rgb(255, 255, 255), window_width / 2, 100, ALLEGRO_ALIGN_CENTER, levelInfo);

    sprintf(levelInfo, "Killed: %d", menu->getKilled());
    al_draw_text(Medium_font, al_map_rgb(255, 255, 255), window_width / 2, 200, ALLEGRO_ALIGN_CENTER, levelInfo);

    sprintf(levelInfo, "Score: %d", menu->getScore());
    al_draw_text(Medium_font, al_map_rgb(255, 255, 255), window_width / 2, 250, ALLEGRO_ALIGN_CENTER, levelInfo);

    sprintf(levelInfo, "Coin: %d", menu->getCoin());
    al_draw_text(Medium_font, al_map_rgb(255, 255, 255), window_width / 2, 300, ALLEGRO_ALIGN_CENTER, levelInfo);

    // if current level is final level, then no need to show next level button anymore
    if(level->getLevel() != LevelNum)
    {
        al_draw_filled_rounded_rectangle(window_width/2 - 150, 400, window_width/2 + 150, 450, 20, 20, ButtonColor);
        al_draw_text(Medium_font, actionColor, window_width / 2, 413, ALLEGRO_ALIGN_CENTER, buttonInfo);
    }

    // show menu button
    al_draw_filled_rounded_rectangle(window_width/2 - 150, 500, window_width/2 + 150, 550, 20, 20, ButtonColor);
    al_draw_text(Medium_font, exitColor, window_width / 2, 513, ALLEGRO_ALIGN_CENTER, "Menu");

    al_flip_display();
}
