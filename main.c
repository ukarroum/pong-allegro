/*
 * ************* Pong ***************
 *
 * Ecrit par Yassir Karroum [ukarroum17@gmail.com]
 * Le 29 juin 2016
 *
 * Un simple jeu de pong implémenté en C avec la bibliothéque Allegro5
 * Il propose deux modes de jeu : un mode solo et un mode multijoueur.
 */

#include <allegro5/allegro.h>
#include <stdio.h>

#include "game.h"

int main()
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue;

    initGame(&display, &queue);
    loopGame(display, queue);
    //destroyGame(&display);

    //printf("%d", minimax());
    return 0;
}