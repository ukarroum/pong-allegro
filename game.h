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
#define BLACK al_map_rgb(0, 0, 0)
#define TRANSPARENT al_map_rgba(0, 0, 0, 0)

/********************
 * ** Enumerations **
 * ******************
 */
enum Mode
{
    SOLO, MULTI
};
typedef enum Mode Mode;

/********************
 * ** Structures ****
 * ******************
 */
enum {KEY_UP,KEY_DOWN, KEY_A, KEY_Q, KEY_MAX};

struct Paddle
{
    ALLEGRO_BITMAP* img;
    int x;
    int y;
    int score;
};
typedef struct Paddle Paddle;

struct Ball
{
    ALLEGRO_BITMAP* img;
    int x;
    int y;
    int dx;
    int dy;

};
typedef struct Ball Ball;

void error(const char *err);
void initGame();
void drawWorld();
void drawMenu();
void loopMenu();
void loopGame();
void updatesScorePlayers();
void lunchBall();
void playComputer();


void updatesScorePlayers();
#endif //PONG_ALLEGRO_GAME_H
