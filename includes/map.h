#ifndef MAP_H
#define MAP_H

#define SCALE 32
#define HEIGHT 22
#define WIDTH 19

#include <SFML/Graphics.h>
#include <string.h>

struct Map {
    sfRectangleShape *wall;
    sfCircleShape *pellet;
    sfCircleShape *powerup;
    int map[HEIGHT][WIDTH];
};

enum MapValues {
    WALL = 4,
    POWER_PELLET = 3,
    REGULAR_PELLET = 2,
    EMPTY = 1
};

struct Map initMap();  // Initialize map structures
void resetMap(struct Map *map);
void drawMap(sfRenderWindow *window, struct Map *map);  // Draw the map with provided structures
int getPelletCount(struct Map *map);  // Get the total number of pellets

#endif