#include "pacman.h"

struct Pacman initPacman()
{
  struct Pacman pacman;
  // Initialize Pacman attributes
  pacman.x = 9;
  pacman.y = 16;
  pacman.direction = 1; // Down
  pacman.lives = 3;
  pacman.score = 0;

  // Create Pacman body
  pacman.body = sfCircleShape_create();
  sfCircleShape_setRadius(pacman.body, (float)(SCALE / 2));
  sfCircleShape_setFillColor(pacman.body, sfYellow);

  // Create Pacman mouth
  pacman.mouth = sfConvexShape_create();
  sfConvexShape_setPointCount(pacman.mouth, 3);
  sfConvexShape_setPoint(pacman.mouth, 0, (sfVector2f){0, 0});
  sfConvexShape_setPoint(pacman.mouth, 1, (sfVector2f){SCALE / 2, -SCALE / 2});
  sfConvexShape_setPoint(pacman.mouth, 2, (sfVector2f){SCALE / 2, SCALE / 2});
  sfConvexShape_setFillColor(pacman.mouth, sfBlack);

  // Return initialized Pacman
  return pacman;
}

void movePacman(struct Pacman *pacman, struct Map *map)
{
  // Move Pacman based on direction
  int nextX = pacman->x;
  int nextY = pacman->y;

  // Determine next position based on direction
  switch (pacman->direction) {
    case 0:  // Right
      if (pacman->x == WIDTH - 1) {nextX = 0;}
      else {nextX++;}
      break;
    case 1:  // Down
      nextY++;
      break;
    case 2:  // Left
      if (pacman->x == 0) {nextX = WIDTH - 1;}
      else {nextX--;}
      break;
    case 3:  // Up
      nextY--;
      break;
  }

  // Check if next position is valid (not a wall) before moving Pacman there
  if (nextX >= 0 && nextX < WIDTH && nextY >= 0 && nextY < HEIGHT && map->map[nextY][nextX] != 4) {
    pacman->x = nextX;
    pacman->y = nextY;
  }
}

void resetPacman(struct Pacman *pacman)
{
  pacman->x = 9;
  pacman->y = 16;
  pacman->direction = 1; 
  pacman->score = 0;
}

void drawPacman(sfRenderWindow *window, struct Pacman player) 
{
  // Set Pacman's body position
  sfCircleShape_setPosition(player.body, (sfVector2f){player.x * SCALE, (player.y + 2) * SCALE});
  // Draw Pacman's body
  sfRenderWindow_drawCircleShape(window, player.body, NULL);

  // Set Pacman's mouth position and rotation
  sfConvexShape_setPosition(player.mouth, (sfVector2f){player.x * SCALE + SCALE / 2, (player.y + 2) * SCALE + SCALE / 2});
  sfConvexShape_setRotation(player.mouth, 90 * player.direction);

  // Only draw Pacman's mouth if the condition is met (e.g., alternating frames)
  if (player.x % 2 == player.y % 2) 
  {
    sfRenderWindow_drawConvexShape(window, player.mouth, NULL);
  }
}