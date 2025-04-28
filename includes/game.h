#ifndef GAME_H
#define GAME_H
#define NUM_GHOSTS 4 
#include <stdio.h>

#include "pacman.h"
#include "ghost.h"

// Enum declarations
enum GameState {
    GAME_START,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER,
    GAME_WIN
};

sfRenderWindow* initWindow();
sfFont* initFont();
sfText* initMessageText(sfFont* font);
sfClock* initClock();
void initGameData(struct Map *gameMap, struct Pacman *player, struct Ghost ghosts[], struct GhostBody ghostBodies[]);
void handleEvent(sfRenderWindow *window, sfEvent event, enum GameState *gameState, struct Pacman *player, struct Ghost ghosts[], struct Map *gameMap, int *pelletCount);

void drawGameMap(sfRenderWindow *window, struct Map *map, struct Ghost ghosts[], struct GhostBody ghostBodies[],  struct Pacman player, int state);
void drawMessage(sfRenderWindow *window, sfText *messageText, sfRectangleShape *messageBg, const char *text, sfColor bgColor, sfColor outlineColor, float offsetX, float offsetY);
void drawUI(sfRenderWindow *window, sfText *messageText, int gameState, int score, int lives);

void move(struct Pacman *pacman, struct Ghost ghosts[], int ghostSize, struct Map* map, int* pelletCount);
int isCollison(struct Pacman *pacman, struct Ghost ghosts[], int ghostSize);
void updateGameLogic(struct Pacman *player, struct Ghost ghosts[], struct Map *gameMap, int *pelletCount, sfClock *clock, enum GameState *gameState);

void cleanup(sfRenderWindow *window, sfClock *clock, struct Map *gameMap, sfText *messageText, sfFont *font, struct GhostBody *ghostBodies);

#endif
