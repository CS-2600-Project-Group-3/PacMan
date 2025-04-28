#ifndef PACMAN_H
#define PACMAN_H

#include "map.h"

// Simple Pacman structure
struct Pacman {
    int x;
    int y;
    int direction; // 0 = right, 1 = down, 2 = left, 3 = up
    int lives;
    int score;
    sfCircleShape *body;
    sfConvexShape *mouth;
};

struct Pacman initPacman();
void movePacman(struct Pacman *pacman, struct Map *map);
void resetPacman(struct Pacman *pacman);
void drawPacman(sfRenderWindow *window, struct Pacman player);

#endif