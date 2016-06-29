#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "game.h"

ALLEGRO_FONT *openSans12 = NULL;
Mode mode = SOLO;
ALLEGRO_BITMAP* player1;
ALLEGRO_BITMAP* player2;

void error(const char *err)
{
    ALLEGRO_DISPLAY *dialogErr;
    dialogErr=al_is_system_installed()?al_get_current_display():NULL;
    al_show_native_message_box(dialogErr,"ERREUR", "Erreur", err,NULL,0);
    exit(EXIT_FAILURE);
}
void initGame(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **queue)
{
    /*******************************
     * ***** Initialisation ********
     * *****************************
     */
    if(!al_init())
        error("al_init()");

    *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    if(!*display)
        error("creation display");

    al_set_window_title(*display, "Pong");

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


    /************************
     * ** Evenements ********
     * **********************
     */

    *queue = al_create_event_queue();

    if(!*queue)
        error("al_create_event_queue");

    al_register_event_source(*queue, al_get_display_event_source(*display));
    al_register_event_source(*queue, al_get_mouse_event_source());
    al_register_event_source(*queue, al_get_keyboard_event_source());

    /*************************************
     * ***** Tracer le menu **************
     * ***********************************
     */
    if(!al_init_primitives_addon())
        error("al_primitives_addon()");

    drawMenu(*display, *queue);
}
void drawMenu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_FONT *openSans62;
    openSans62 = al_load_ttf_font("../fonts/OpenSans-Light.ttf", 62, 0);
    if(!openSans62)
        error("al_load_font()");

    al_draw_text(openSans62, WHITE, SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.1, 0, "Pong");

    al_draw_text(openSans12, WHITE, SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.5, 0, "1)    Solo");

    al_draw_text(openSans12, WHITE, SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.6, 0, "2)    Multiplayer");

    al_flip_display();

    loopMenu(display, queue);
};
void loopMenu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue)
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
                    drawWorld(display);
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                    mode = MULTI;
                    fin = 0;
                    drawWorld(display);
                }
                break;
        }
    }
}
void drawWorld(ALLEGRO_DISPLAY *display)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    /******************* Dessin de la ligne pointillé ****************/
    for (int i = 0; i < GAME_HEIGHT; i+= 20)
        al_draw_line(GAME_WIDTH/2, i, GAME_WIDTH/2, i + 10, WHITE, 3);

    //Dessin des deux raquettes virtuelles
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

    player1 = al_create_bitmap(10, 60);
    player2 = al_create_bitmap(10, 60);

    if (!player1 || !player2)
        error("al_create_bitmap()");

    al_set_target_bitmap(player1);
    al_clear_to_color(WHITE);
    al_set_target_backbuffer(display);
    al_draw_bitmap(player1, 20, GAME_HEIGHT/2, 0);

    al_set_target_bitmap(player2);
    al_clear_to_color(WHITE);
    al_set_target_backbuffer(display);
    al_draw_bitmap(player2, GAME_WIDTH - 20 - 10, GAME_HEIGHT/2, 0);

    updatesScorePlayers(0, 0);

    al_flip_display();

    //srand(time(NULL));
}
void loopGame(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue)
{
    int fin = 1;
    while(fin)
    {
        ALLEGRO_EVENT event = {0};

        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fin = 0;
                break;
        }
    }
}
void updatesScorePlayers(int player1, int player2)
{
    if(mode == MULTI){
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 10, 0, "Joueur 1 : %d", player1);
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 50, 0, "Joueur 2 : %d", player2);
    }
    else{
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 10, 0, "Joueur : %d", player1);
        al_draw_textf(openSans12, WHITE, SCREEN_WIDTH*0.1, GAME_HEIGHT + 50, 0, "Machine : %d", player2);
    }

    al_flip_display();
}