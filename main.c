#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.h>
#include "./includes/game.h"
//#include "c:/MYDOWNLOADS/Coding/000 Scripts and Files/CS 2600/Pacman/CSFML/include/SFML/Graphics.h"

int main() 
{
    enum GameState gameState = GAME_START;

    // Initialize resources
    sfRenderWindow *window = initWindow();
    sfFont* font = initFont();
    sfText* messageText = initMessageText(font);
    sfClock *clock = initClock();

    // Initialize game data
    struct Map gameMap;
    struct Pacman player;
    struct Ghost ghosts[4];
    struct GhostBody ghostBodies[4];
    int pelletCount = 0;
    initGameData(&gameMap, &player, ghosts, ghostBodies);
    pelletCount = getPelletCount(&gameMap);

    while (sfRenderWindow_isOpen(window)) 
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) 
        {
            handleEvent(window, event, &gameState, &player, ghosts, &gameMap, &pelletCount);  // Handle events
        }
        if (gameState == GAME_PLAYING) 
        {
            updateGameLogic(&player, ghosts, &gameMap, &pelletCount, clock, &gameState);
        }
        
        drawGameMap(window, &gameMap, ghosts, ghostBodies, player, gameState);  // draw map
        drawUI(window, messageText, gameState, player.score, player.lives, &player);     // UI based on game state
            
        if (pelletCount == 0)  // Check for game end
        { 
            gameState = GAME_WIN;
        }
    }
    cleanup(window, clock, &gameMap, messageText, font, ghostBodies);  // Cleanup (deallocate resources)

    return 0;
}