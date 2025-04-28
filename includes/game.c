#include "game.h"


int isCollison(struct Pacman *pacman, struct Ghost ghosts[], int ghostSize)
{
  for (int i = 0; i < ghostSize; i++) 
  {
    if (pacman->x == ghosts[i].x && pacman->y == ghosts[i].y) 
    {
      return 1;
    }
  }
  return 0;
}

void move(struct Pacman *pacman, struct Ghost ghosts[], int ghostSize, struct Map *map, int* pelletCount)
{
    movePacman(pacman, map);  // move pacman
    if(isCollison(pacman, ghosts, ghostSize)) // Check if pac-man is on a ghost (before moving ghosts)
    {
        pacman->lives --;
        resetPacman(pacman);
        resetGhosts(ghosts);
        
    }
    for (int i = 0; i < ghostSize; i++) {  // Move Ghosts
        moveGhost(&ghosts[i], map);
    }
    if(isCollison(pacman, ghosts, ghostSize)) // Check if pac-man is on a ghost (after moving ghosts)
    {
        pacman->lives --;
        resetPacman(pacman);
        resetGhosts(ghosts);
    }

    if (map->map[pacman->y][pacman->x] == REGULAR_PELLET || map->map[pacman->y][pacman->x] == POWER_PELLET) // Check if pac-man is on a pellet or powerup
    {
        pacman->score += (map->map[pacman->y][pacman->x] == POWER_PELLET) ? 50 : 10;
        (*pelletCount)--;
        map->map[pacman->y][pacman->x] = 1;
    }
}

void drawGameMap(sfRenderWindow *window, struct Map *map, struct Ghost ghosts[], struct GhostBody ghostBodies[] ,struct Pacman player, int state)
{
    // Clear window
    sfRenderWindow_clear(window, sfBlack);
    
    drawMap(window, map);  // Draw Map

    if (state != GAME_START) 
    {
        drawPacman(window, player);  // Draw Pac Man
        for (int i = 0; i < 4; i++)  // Draw Ghosts
        {
            drawGhost(window, ghosts[i], ghostBodies[i]);
        }
    }

}

void drawMessage(sfRenderWindow *window, sfText *messageText, sfRectangleShape *messageBg, const char *text, sfColor bgColor, sfColor outlineColor, float offsetX, float offsetY) {
    // Set the message text
    sfText_setString(messageText, text);

    // Calculate bounds for centering
    sfFloatRect textBounds = sfText_getLocalBounds(messageText);
    sfVector2f centerPos = {
        (WIDTH * SCALE - textBounds.width) / 2 + offsetX,
        ((HEIGHT + 2) * SCALE - textBounds.height) / 2 + offsetY
    };
    sfText_setPosition(messageText, centerPos);

    // Set rectangle size and position
    sfRectangleShape_setFillColor(messageBg, bgColor);  // Background color
    sfRectangleShape_setOutlineColor(messageBg, outlineColor);  // Outline color
    sfRectangleShape_setSize(messageBg, (sfVector2f){textBounds.width + 40, textBounds.height + 40});
    sfRectangleShape_setPosition(messageBg, (sfVector2f){centerPos.x - 20, centerPos.y - 10});

    // Draw the background and text
    sfRenderWindow_drawRectangleShape(window, messageBg, NULL);
    sfRenderWindow_drawText(window, messageText, NULL);
}

void drawUI(sfRenderWindow *window, sfText *messageText, int gameState, int score, int lives)
{
    // UI based on game state
    char scoreText[50];
    sprintf(scoreText, "Score: %d   Lives: %d", score, lives);
    sfText_setString(messageText, scoreText);

    // Center score text
    sfFloatRect scoreBounds = sfText_getLocalBounds(messageText);
    sfVector2f scorePos = {
        (WIDTH * SCALE - scoreBounds.width) / 2,
        SCALE / 2
    };
    sfText_setPosition(messageText, scorePos);
    sfRenderWindow_drawText(window, messageText, NULL);

    // message backing rectangle
    sfRectangleShape* messageBg = sfRectangleShape_create();
    sfRectangleShape_setFillColor(messageBg, (sfColor){0, 0, 0, 200});
    sfRectangleShape_setOutlineThickness(messageBg, 2.0f);
    sfRectangleShape_setOutlineColor(messageBg, sfBlue);
    
    // Game State Message
    switch (gameState) 
    {
        case GAME_START:
            drawMessage(window, messageText, messageBg, "PACMAN\nPress SPACE\nto start", (sfColor){0, 0, 0, 200}, sfBlue, 0, -20);
            break;
        case GAME_PAUSED:
            drawMessage(window, messageText, messageBg, "PAUSED\nPress SPACE\nto continue", (sfColor){0, 0, 0, 200}, sfYellow, 0, -20);
            break;
        case GAME_OVER:
            drawMessage(window, messageText, messageBg, "GAME OVER\nPress SPACE\nto restart", (sfColor){0, 0, 0, 220}, sfRed, 0, -20);
            break;
        case GAME_WIN:
            drawMessage(window, messageText, messageBg, "YOU WIN!\nPress SPACE\nto play again", (sfColor){0, 0, 0, 220}, sfGreen, 0, -20);
            break;
        default:
            break;
    }
    sfRectangleShape_destroy(messageBg);
    sfRenderWindow_display(window);  // Update window
}

void cleanup(sfRenderWindow *window, sfClock *clock, struct Map *gameMap, sfText *messageText, sfFont *font, struct GhostBody *ghostBodies) 
{
    // Deallocate resources
    sfRenderWindow_destroy(window);
    sfClock_destroy(clock);
    
    // Cleanup Map resources
    sfRectangleShape_destroy(gameMap->wall);
    sfCircleShape_destroy(gameMap->pellet);
    sfCircleShape_destroy(gameMap->powerup);

    // Cleanup UI resources
    sfText_destroy(messageText);
    sfFont_destroy(font);

    // Cleanup Ghost Body resources
    for (int i = 0; i < 4; i++) {
        sfConvexShape_destroy(ghostBodies[i].body);
    }
}


sfRenderWindow* initWindow() 
{
    sfVideoMode mode = {WIDTH * SCALE, (HEIGHT + 2) * SCALE, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Pac Man", sfResize | sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);
    return window;
}


sfFont* initFont() {
    return sfFont_createFromFile("resources/fonts/slkscr.ttf");
}

sfText* initMessageText(sfFont* font) {
    sfText* messageText = sfText_create();
    sfText_setFont(messageText, font);
    sfText_setCharacterSize(messageText, 24);
    sfText_setFillColor(messageText, sfWhite);
    return messageText;
}

sfClock* initClock() {
    return sfClock_create();
}

void initGameData(struct Map *gameMap, struct Pacman *player, struct Ghost ghosts[], struct GhostBody ghostBodies[]) {
    *gameMap = initMap();
    *player = initPacman();

    ghosts[0] = (struct Ghost){9, 9, 0, 0, 'r'};
    ghosts[1] = (struct Ghost){8, 10, 0, 0, 'b'};
    ghosts[2] = (struct Ghost){9, 10, 0, 0, 'p'};
    ghosts[3] = (struct Ghost){10, 10, 0, 0, 'o'};

    for (int i = 0; i < 4; i++) 
    {
        ghostBodies[i] = makeBody();
    }
}

// Function to handle events
void handleEvent(sfRenderWindow *window, sfEvent event, enum GameState *gameState, struct Pacman *player, struct Ghost ghosts[], struct Map *gameMap, int *pelletCount) {
    switch (event.type) 
    {
        case sfEvtClosed:
            sfRenderWindow_close(window);
            break;

        case sfEvtKeyPressed:
            if (event.key.code == sfKeySpace)
            {
                switch (*gameState)
                {
                    case GAME_START:  // game just started
                        *gameState = GAME_PLAYING;
                        break;
                    case GAME_PAUSED:  // game paused, go into play mode
                        *gameState = GAME_PLAYING;
                        break;
                    case GAME_PLAYING:  // game playing, go into pause mode 
                        *gameState = GAME_PAUSED;
                        break;
                    case GAME_OVER:
                    case GAME_WIN:
                        player->lives = 3;
                        resetPacman(player);                    // Reset pacman
                        resetGhosts(ghosts);                     // Reset ghosts
                        resetMap(gameMap);                      // Rest Map
                        *pelletCount = getPelletCount(gameMap);  // Rest pelletCount
                        *gameState = GAME_START;                  // Rest gameState
                        break;
                }  // switch (gameState)
            }  // if (event.key.code == sfKeySpace)
            break;

        default:
            break;
    }  // switch (event.type) 

    if (*gameState == GAME_PLAYING) {  // input handling movement only during playing state
        switch (event.key.code) {
            case sfKeyUp:
                player->direction = 3;
                break;
            case sfKeyDown:
                player->direction = 1;
                break;
            case sfKeyLeft:
                player->direction = 2;
                break;
            case sfKeyRight:
                player->direction = 0;
                break;
            default:
                break;
        }
    }
}

void updateGameLogic(struct Pacman *player, struct Ghost ghosts[], struct Map *gameMap, int *pelletCount, sfClock *clock, enum GameState *gameState) 
{
    sfTime time = sfClock_getElapsedTime(clock);
    float seconds = sfTime_asSeconds(time);
    if (seconds >= 0.1) {
        move(player, ghosts, NUM_GHOSTS, gameMap, pelletCount);
        
        if (player->lives <= 0) {
            *gameState = GAME_OVER;
        }
        if (*pelletCount == 0) {
            *gameState = GAME_WIN;
        }
        sfClock_restart(clock); 
    }
}
