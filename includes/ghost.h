#ifndef GHOST_H
#define GHOST_H

#include <stdlib.h>
#include "map.h"

// Ghost Structure
struct Ghost {
    int x;
    int y;
    int direction; // 0 = right, 1 = down, 2 = left, 3 = up
    int status; // 0 = aggressive, 1 = afraid, 2 = running
    char color; // r = red, b = blue, p = pink, o = orange
};

struct GhostBody {
  sfConvexShape *body;
  sfCircleShape *eye;
  sfCircleShape *pupil;
};


struct GhostBody makeBody();
int getNextX(int direction, int x);
int getNextY(int direction, int y);
int rotate(int direction, int clockwiseSteps);
void moveGhost(struct Ghost *ghost, struct Map *gameMap) ;
void resetGhosts(struct Ghost ghosts[]);
void drawGhost(sfRenderWindow *window, struct Ghost ghost, struct GhostBody ghostBody);

#endif