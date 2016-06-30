#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "game.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *queue = NULL;
ALLEGRO_TIMER*timer;

ALLEGRO_FONT *openSans12 = NULL;
Mode mode = SOLO;
Paddle player1 = {0};
Paddle player2 = {0};
Ball ball = {0};

void error(const char *err)
{
    ALLEGRO_DISPLAY *dialogErr;
    dialogErr=al_is_system_installed()?al_get_current_display():NULL;
    al_show_native_message_box(dialogErr,"ERREUR", "Erreur", err,NULL,0);
    exit(EXIT_FAILURE);
}
void initGame()
{
    /*******************************
     * ***** Initialisation ********
     * *****************************
     */
    if(!al_init())
        error("al_init()");

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    if(!display)
        error("creation display");

    al_set_window_title(display, "Pong");

    if(!al_install_mouse())
        error("al_install_keyboard()");

    if(!al_install_keyboard())
        error("al_install_keyboard()");

    if(!al_init_font_addon())
        error("al_init_font_addon()");

    if(!al_init_ttf_addon())
        error("al_init_ttf_addon()");

    openSans12 = al_load_ttf_font("../fonts/OpenSans-Light.ttf", 24, 0);
    if(!openSans12)
        error("al_load_font()");

    timer = al_create_timer(1.0 / 30);
    if (!timer)
        error("al_create_timer()");

    if(!al_init_primitives_addon())
        error("al_primitives_addon()");

    /************************
     * ** Evenements ********
     * **********************
     */

    queue = al_create_event_queue();

    if(!queue)
        error("al_create_event_queue");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    /* Definition des raquettes des deux joueurs */

    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

    player1.img = al_create_bitmap(10, 60);
    player2.img = al_create_bitmap(10, 60);

    if (!player1.img || !player2.img)
        error("al_create_bitmap()");

    player1.x = 20;
    player1.y = GAME_HEIGHT/2;
    player1.score = 0;
    al_set_target_bitmap(player1.img);
    al_clear_to_color(WHITE);
    al_set_target_backbuffer(display);

    player2.x = GAME_WIDTH - 20 - 10;
    player2.y = GAME_HEIGHT/2;
    player2.score = 0;
    al_set_target_bitmap(player2.img);
    al_clear_to_color(WHITE);
    al_set_target_backbuffer(display);

    // La balle

    ball.img = al_create_bitmap(14, 14);
    ball.x = -1;
    ball.y = -1;
    ball.dx = 0;
    ball.dy = 0;

    al_set_target_bitmap(ball.img);
    al_clear_to_color(TRANSPARENT);
    al_draw_filled_circle(7, 7, 7, WHITE);
    al_set_target_backbuffer(display);



    /*************************************
     * ***** Tracer le menu **************
     * ***********************************
     */

    al_start_timer(timer);

    srand(time(NULL));


    drawMenu();
}
void drawMenu()
{
    ALLEGRO_FONT *openSans62;
    openSans62 = al_load_ttf_font("../fonts/OpenSans-Light.ttf", 62, 0);
    if(!openSans62)
        error("al_load_font()");

    al_draw_text(openSans62, WHITE, SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.1, 0, "Pong");

    al_draw_text(openSans12, WHITE, SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.5, 0, "1)    Solo");

    al_draw_text(openSans12, WHITE, SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.6, 0, "2)    Multiplayer");

    al_flip_display();

    loopMenu();
};
void loopMenu()
{
    int fin = 1;
    while(fin)
    {
        ALLEGRO_EVENT event = {0};

        al_wait_for_event(queue, &event);

        switch(event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fin = 0;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_1) {
                    mode = SOLO;
                    fin = 0;
                    drawWorld();
                    lunchBall();
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                    mode = MULTI;
                    fin = 0;
                    drawWorld();
                    lunchBall();
                }
                break;
        }
    }
}
void drawWorld()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    /******************* Dessin de la ligne pointillé ****************/
    for (int i = 0; i < GAME_HEIGHT; i+= 20)
        al_draw_line(GAME_WIDTH/2, i, GAME_WIDTH/2, i + 10, WHITE, 3);

    //Dessin des deux raquettes virtuelles

    if(player1.y < 0)
        player1.y = 0;
    if(player1.y > GAME_HEIGHT - 60)
        player1.y = GAME_HEIGHT - 60;
    if(player2.y < 0)
        player2.y = 0;
    if(player2.y > GAME_HEIGHT - 60)
        player2.y = GAME_HEIGHT - 60;

    al_draw_bitmap(player1.img, player1.x, player1.y, 0);
    al_draw_bitmap(player2.img, player2.x, player2.y, 0);

    //Dessin de la balle

    /****** Test si la ball a atteint le bas ou le haut de l'ecran si c'est le case je change ca direction ******/
    if(ball.y < 0)
    {
        ball.y = 0;
        ball.dy *= -1;
    }
    if(ball.y > GAME_HEIGHT - 14)
    {
        ball.y = GAME_HEIGHT - 14;
        ball.dy *= -1;
    }

    /********* gestion des collisions ********/
    if(ball.x <= player1.x + 10 && ball.x >= player1.x && ball.y >= player1.y && ball.y + 7 <= player1.y + 60)
    {
        ball.dx *= -1;
    }
    if(ball.x + 14 <= player2.x + 10 && ball.x + 14 >= player2.x && ball.y >= player2.y && ball.y + 7 <= player2.y + 60)
    {
        ball.dx *= -1;
    }

    al_draw_bitmap(ball.img, ball.x, ball.y, 0);

    if(ball.x + 14 < 0)
    {
        al_stop_timer(timer);
        if(mode == MULTI)
            al_show_native_message_box(al_get_current_display(),"Bravo !", "Bravo !", "Vous avez gagné !",NULL,0);
        else
            al_show_native_message_box(al_get_current_display(),"Pas de chance !", "Pad de chance !", "Vous avez perdu :( !",NULL,0);
        player2.score++;
        lunchBall();
        al_start_timer(timer);
    }
    else if(ball.x > GAME_WIDTH)
    {
        al_stop_timer(timer);
        al_show_native_message_box(al_get_current_display(),"Bravo !", "Bravo !", "Vous avez gagné !",NULL,0);
        player1.score++;
        lunchBall();
        al_start_timer(timer);
    }

    //Affichage des scores
    updatesScorePlayers();

    al_flip_display();
}
void loopGame()
{
    bool fin = true;
    bool key[KEY_MAX]={0};

    while(fin) {
        ALLEGRO_EVENT event = {0};

        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fin = false;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[KEY_UP] = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        key[KEY_DOWN] = true;
                        break;
                    case ALLEGRO_KEY_A:
                        key[KEY_A] = true;
                        break;
                    case ALLEGRO_KEY_Q:
                        key[KEY_Q] = true;
                        break;
                }
                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[KEY_UP] = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        key[KEY_DOWN] = false;
                        break;
                    case ALLEGRO_KEY_A:
                        key[KEY_A] = false;
                        break;
                    case ALLEGRO_KEY_Q:
                        key[KEY_Q] = false;
                        break;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                player1.y -= key[KEY_UP] * 10;
                player1.y += key[KEY_DOWN] * 10;
                if(mode == MULTI)
                {
                    player2.y -= key[KEY_A] * 10;
                    player2.y += key[KEY_Q] * 10;
                }
                ball.x += ball.dx;
                ball.y += ball.dy;
                if(mode == SOLO)
                    playComputer();
                if(al_is_event_queue_empty(queue))
                    drawWorld();
                break;
        }
    }
}
void updatesScorePlayers()
{
    if(mode == MULTI){
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 10, 0, "Joueur 1 : %d", player1.score);
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 50, 0, "Joueur 2 : %d", player2.score);
    }
    else{
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 10, 0, "Joueur : %d", player1.score);
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 50, 0, "Machine : %d", player2.score);
    }

    al_flip_display();
}
void lunchBall()
{
    ball.x = GAME_WIDTH/2;
    ball.y = GAME_HEIGHT/2;
    /****** les valeurs sont dans l'intervalle [3, 6] (trouvé par experimentation) *********/
    ball.dx = -3 - rand()%3;
    if(rand()%1 == 0)
        ball.dy = 3 + rand()%3;
    else
        ball.dy = -3 - rand()%3;

    player1.y = GAME_WIDTH/2;
    player2.y = GAME_WIDTH/2;
}
void playComputer()
{
    /*********************************************
     * ** Deplace la raquette dans le mode solo***
     * *******************************************
     */
    /*
     * Si la balle est dans la partie gauche(celle de l'adversaire) la raquette reste immobile.
     * SI la balle se dirige vers la partie gauche la raquette reste immobile (sinon le jeu devient bien plus difficile)
     */
    if(ball.x < GAME_WIDTH/2 || ball.dx < 0)
        return;

    if(ball.y > player2.y + 30)
        player2.y += 4 + rand() % 2;
    else if(ball.y + 14 < player2.y + 30)
        player2.y -= 4 + rand() % 2;
}