#ifndef PONG_ALLEGRO_GAME_H
#define PONG_ALLEGRO_GAME_H

#include <allegro5/allegro_font.h>

#define DEBUG //définir si en phase de débogage

#define GAME_WIDTH 400
#define GAME_HEIGHT 400
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 500
#define RESSOURCES_PATH ..

/*********************
 * *** Couleurs ******
 * *******************
 */
#define WHITE al_map_rgb( 255, 255, 255)

/********************
 * ** Enumerations **
 * ******************
 */
enum Mode
{
    SOLO, MULTI
};
typedef enum Mode Mode;


void error(const char *err);
void initGame(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **queue);
void drawWorld(ALLEGRO_DISPLAY *display);
void drawMenu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue);
void loopMenu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue);
void loopGame(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue);

void updatesScorePlayers(int player1, int player2);
#endif //PONG_ALLEGRO_GAME_H
