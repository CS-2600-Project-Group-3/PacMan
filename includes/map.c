#include "map.h"

static const int initialMap[HEIGHT][WIDTH] = {
  {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
  {4, 3, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 3, 4},
  {4, 2, 4, 4, 2, 4, 4, 4, 2, 4, 2, 4, 4, 4, 2, 4, 4, 2, 4},
  {4, 2, 4, 4, 2, 4, 4, 4, 2, 4, 2, 4, 4, 4, 2, 4, 4, 2, 4},
  {4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4},
  {4, 2, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 2, 4, 2, 4, 4, 2, 4},
  {4, 2, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2, 2, 2, 4},
  {4, 4, 4, 4, 2, 4, 4, 4, 2, 4, 2, 4, 4, 4, 2, 4, 4, 4, 4},
  {1, 1, 1, 4, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 4, 1, 1, 1},
  {4, 4, 4, 4, 2, 4, 2, 4, 4, 1, 4, 4, 2, 4, 2, 4, 4, 4, 4},
  {2, 2, 2, 2, 2, 2, 2, 4, 1, 1, 1, 4, 2, 2, 2, 2, 2, 2, 2},
  {4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4},
  {1, 1, 1, 4, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 4, 1, 1, 1},
  {4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4},
  {4, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 4},
  {4, 2, 4, 4, 2, 4, 4, 4, 2, 4, 2, 4, 4, 4, 2, 4, 4, 2, 4},
  {4, 2, 2, 4, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 4, 2, 2, 4},
  {4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4},
  {4, 2, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2, 2, 2, 4},
  {4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4},
  {4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4},
  {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
};

struct Map initMap()
{
    struct Map map;

    // Copy the initial map values to the map array in the struct
    memcpy(map.map, initialMap, sizeof(initialMap));

    // Create rectangle for walls
    map.wall = sfRectangleShape_create();
    sfVector2f rectSize = {(float)SCALE, (float)SCALE};
    sfRectangleShape_setSize(map.wall, rectSize);
    sfRectangleShape_setFillColor(map.wall, (sfColor){0, 0, 255, 255});

    // Create dot for pellets
    map.pellet = sfCircleShape_create();
    sfCircleShape_setRadius(map.pellet, (float)(SCALE / 8));

    // Create dot for powerups
    map.powerup = sfCircleShape_create();
    sfCircleShape_setRadius(map.powerup, (float)(SCALE / 5));

    return map;
}

void resetMap(struct Map *map)
{
    // Copy the initial map values to the map array in the struct
    memcpy(map->map, initialMap, sizeof(initialMap));
}

void drawMap(sfRenderWindow *window, struct Map *gameMap) {
  for (int y = 0; y < HEIGHT; y++) 
  {
    for (int x = 0; x < WIDTH; x++) 
    {
      int value = gameMap->map[y][x];
      if (value == WALL) { // Wall
        sfRectangleShape_setPosition(gameMap->wall, (sfVector2f){x * SCALE, (y + 2) * SCALE});
        sfRenderWindow_drawRectangleShape(window, gameMap->wall, NULL);
      }
      else if (value == POWER_PELLET) { // Powerup
        sfCircleShape_setPosition(gameMap->powerup, (sfVector2f){x * SCALE + SCALE / 2 - sfCircleShape_getRadius(gameMap->powerup), (y + 2) * SCALE + SCALE / 2 - sfCircleShape_getRadius(gameMap->powerup)});
        sfRenderWindow_drawCircleShape(window, gameMap->powerup, NULL);
      }
      else if (value == REGULAR_PELLET) { // Pellet
        sfCircleShape_setPosition(gameMap->pellet, (sfVector2f){x * SCALE + SCALE / 2 - sfCircleShape_getRadius(gameMap->pellet), (y + 2) * SCALE + SCALE / 2 - sfCircleShape_getRadius(gameMap->pellet)});
        sfRenderWindow_drawCircleShape(window, gameMap->pellet, NULL);
      }
    }
  }
}

int getPelletCount(struct Map *map)
{
  int pelletCount = 0;
  for (int i = 0; i < HEIGHT; i++) 
  {
    for (int j = 0; j < WIDTH; j++) 
    {
      if (map->map[i][j] == REGULAR_PELLET || map->map[i][j] == POWER_PELLET)
      {
        pelletCount++;
      } 
    }
  }
  return pelletCount;
}
