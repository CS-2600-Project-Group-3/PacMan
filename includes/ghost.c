#include "ghost.h"
#include<unistd.h>

int ms = 10000; //delay for 10,000 miliseconds

struct GhostBody makeBody()
{
  struct GhostBody ghostBody;
  ghostBody.body = sfConvexShape_create();
  sfConvexShape_setPointCount(ghostBody.body, 9);
  sfConvexShape_setPoint(ghostBody.body, 0, (sfVector2f){SCALE / 4, 0});
  sfConvexShape_setPoint(ghostBody.body, 1, (sfVector2f){SCALE * 3 / 4, 0});
  sfConvexShape_setPoint(ghostBody.body, 2, (sfVector2f){SCALE, SCALE / 4});
  sfConvexShape_setPoint(ghostBody.body, 3, (sfVector2f){SCALE, SCALE});
  sfConvexShape_setPoint(ghostBody.body, 4, (sfVector2f){SCALE * 3 / 4, SCALE * 3 / 4});
  sfConvexShape_setPoint(ghostBody.body, 5, (sfVector2f){SCALE /2, SCALE});
  sfConvexShape_setPoint(ghostBody.body, 6, (sfVector2f){SCALE / 4, SCALE * 3 / 4});
  sfConvexShape_setPoint(ghostBody.body, 7, (sfVector2f){0, SCALE});
  sfConvexShape_setPoint(ghostBody.body, 8, (sfVector2f){0, SCALE / 4});
  ghostBody.pupil = sfCircleShape_create();
  sfCircleShape_setRadius(ghostBody.pupil, (float)(SCALE / 8));
  sfCircleShape_setFillColor(ghostBody.pupil, sfBlack);
  ghostBody.eye = sfCircleShape_create();
  sfCircleShape_setRadius(ghostBody.eye, (float)(SCALE / 4));

  return ghostBody;
}

int getNextX(int direction, int x) {
    return (WIDTH + (x - ((direction - 1) % 2))) % WIDTH;
}

int getNextY(int direction, int y) {
    return y + ((2 - direction) % 2);
}

int rotate(int direction, int clockwiseSteps) {
  return (4 + direction + clockwiseSteps) % 4;
}

void moveGhost(struct Ghost *ghost, struct Map *gameMap) 
{
  int validDirs[3] = {0, 0, 0}; // -1 = left, 0 = straight, 1 = right
  int numValidDirs = 0;

  // Populate validDirs (check if directions are valid based on walls)
  for (int j = 0; j < 3; j++) {
    if (gameMap->map[getNextY(rotate(ghost->direction, j - 1), ghost->y)][getNextX(rotate(ghost->direction, j - 1), ghost->x)] != 4) {
      //usleep(ms);
      validDirs[numValidDirs] = j - 1;
      numValidDirs++;
    }
  }

  // If left and right side have walls
  if (gameMap->map[getNextY(rotate(ghost->direction, 1), ghost->y)][getNextX(rotate(ghost->direction, 1), ghost->x)] == 4 &&
    gameMap->map[getNextY(rotate(ghost->direction, -1), ghost->y)][getNextX(rotate(ghost->direction, -1), ghost->x)] == 4) {
    usleep(ms);
      // If front has a wall, turn around
    if (gameMap->map[getNextY(ghost->direction, ghost->y)][getNextX(ghost->direction, ghost->x)] == 4) {
      usleep(ms);
      ghost->direction = rotate(ghost->direction, 2); // Turn around
    }
  }
  // Else pick a random valid direction at an intersection
  else if (numValidDirs > 0) {
    usleep(ms);
    ghost->direction = rotate(ghost->direction, validDirs[rand() % numValidDirs]);
  }

  // Move ghost
  ghost->x = getNextX(ghost->direction, ghost->x);
  ghost->y = getNextY(ghost->direction, ghost->y);
}

void resetGhosts(struct Ghost ghosts[])
{
  ghosts[0] = (struct Ghost){9, 9, 0, 0, 'r'};
  ghosts[1] = (struct Ghost){8, 10, 0, 0, 'b'};
  ghosts[2] = (struct Ghost){9, 10, 0, 0, 'p'};
  ghosts[3] = (struct Ghost){10, 10, 0, 0, 'o'};
}

void drawGhost(sfRenderWindow *window, struct Ghost ghost, struct GhostBody ghostBody)
{
  //ghost color based on status
  if (ghost.status == 1) { //afraid state: blue 
      sfConvexShape_setFillColor(ghostBody.body, (sfColor){0, 0, 255, 255});
  } else if (ghost.status == 2) { //running: white
      sfConvexShape_setFillColor(ghostBody.body, (sfColor){255, 255, 255, 255});
  } else { //normal
      switch (ghost.color) 
      {
        case 'r':
            sfConvexShape_setFillColor(ghostBody.body, sfRed);
            break;
        case 'b':
            sfConvexShape_setFillColor(ghostBody.body, (sfColor){144, 213, 255, 255});
            break;
        case 'p':
            sfConvexShape_setFillColor(ghostBody.body, (sfColor){255, 182, 193, 255});
            break;
        case 'o':
            sfConvexShape_setFillColor(ghostBody.body, (sfColor){255, 165, 0, 255});
            break;
        default:
            break;
      }
  }

    sfConvexShape_setPosition(ghostBody.body, (sfVector2f){ghost.x * SCALE, (ghost.y + 2) * SCALE});
    sfRenderWindow_drawConvexShape(window, ghostBody.body, NULL);
    sfCircleShape_setPosition(ghostBody.eye, (sfVector2f){ghost.x * SCALE, (ghost.y + 2) * SCALE + SCALE / 6});
    sfRenderWindow_drawCircleShape(window, ghostBody.eye, NULL);
    sfCircleShape_setPosition(ghostBody.eye, (sfVector2f){ghost.x * SCALE + SCALE / 2, (ghost.y+2) * SCALE + SCALE / 6});
    sfRenderWindow_drawCircleShape(window, ghostBody.eye, NULL);
    sfVector2f leftEyeDirection;
    sfVector2f rightEyeDirection;
    switch(ghost.direction) {
        case 0:
            leftEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.3, (ghost.y + 2) * SCALE + SCALE * 0.3};
	    usleep(ms);
            rightEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.8, (ghost.y + 2) * SCALE + SCALE * 0.3};
	    usleep(ms);
            break;
        case 1:
            leftEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.15, (ghost.y + 2) * SCALE + SCALE * 0.45};
	    usleep(ms);
            rightEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.65, (ghost.y + 2) * SCALE + SCALE * 0.45};
	    usleep(ms);
            break;
        case 2:
            leftEyeDirection = (sfVector2f){ghost.x * SCALE, (ghost.y + 2) * SCALE + SCALE * 0.3};
	    usleep(ms);
            rightEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.5, (ghost.y + 2) * SCALE + SCALE * 0.3};
	    usleep(ms);
            break;
        default:
            leftEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.15, (ghost.y + 2) * SCALE + SCALE * 0.15};
	    usleep(ms);
            rightEyeDirection = (sfVector2f){ghost.x * SCALE + SCALE * 0.65, (ghost.y + 2) * SCALE + SCALE * 0.15};
	    usleep(ms);
            break;
    }
    sfCircleShape_setPosition(ghostBody.pupil, leftEyeDirection);
    sfRenderWindow_drawCircleShape(window, ghostBody.pupil, NULL);
    sfCircleShape_setPosition(ghostBody.pupil, rightEyeDirection);
    sfRenderWindow_drawCircleShape(window, ghostBody.pupil, NULL);
}
