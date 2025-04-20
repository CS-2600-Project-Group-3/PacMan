#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.h>
//#include "c:/MYDOWNLOADS/Coding/000 Scripts and Files/CS 2600/Pacman/CSFML/include/SFML/Graphics.h"

// Simple Pacman structure
struct Pacman {
    int x;
    int y;
    int direction; // 0 = right, 1 = down, 2 = left, 3 = up
    int lives;
    int score;
};

// Ghost Structure
struct Ghost {
    int x;
    int y;
    int direction; // 0 = right, 1 = down, 2 = left, 3 = up
    int status; // 0 = aggressive, 1 = afraid, 2 = running
    char color; // r = red, b = blue, p = pink, o = orange
};

enum GameState {
    GAME_START,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER,
    GAME_WIN
};

enum MapDimensions {
    HEIGHT = 22,
    WIDTH =19
};
const int SCALE = 20;

// 1 = Nothing, 2 = Dot, 3 = Power Up, 4 = Wall, 5 = Ghost Home
int map[HEIGHT][WIDTH] = {
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

int getNextX(int direction, int x) {
    return (WIDTH + (x - ((direction - 1) % 2))) % WIDTH;
};

int getNextY(int direction, int y) {
    return y + ((2 - direction) % 2);
}

int rotate(int direction, int clockwiseSteps) {
    return (4 + direction + clockwiseSteps) % 4;
}

int main() {
    enum GameState gameState = GAME_START;

    //Create window
    sfVideoMode mode = {WIDTH * SCALE, HEIGHT * SCALE, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Pac Man", sfResize | sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);

    //for fonts & UI text
    sfFont* font = sfFont_createFromFile("c:\\Windows\\Fonts\\arial.ttf");
    sfText* messageText = sfText_create();
    sfText_setFont(messageText, font);
    sfText_setCharacterSize(messageText, 24);
    sfText_setFillColor(messageText, sfWhite);

    //Create clock
    sfClock *clock = sfClock_create();

    //Generate RNG seed
    srand(time(NULL));

    //Create rectangle for walls
    sfRectangleShape *wall = sfRectangleShape_create();
    sfVector2f rectSize = {(float)SCALE, (float)SCALE};
    sfRectangleShape_setSize(wall, rectSize);
    sfRectangleShape_setFillColor(wall, (sfColor){0, 0, 255, 255});

    //Create dot for pellets
    sfCircleShape *pellet = sfCircleShape_create();
    sfCircleShape_setRadius(pellet, (float)(SCALE / 8));

    //Create dot for powerups
    sfCircleShape *powerup = sfCircleShape_create();
    sfCircleShape_setRadius(powerup, (float)(SCALE / 5));

    //intialize Pacman character
    struct Pacman player;
    player.x = 9;
    player.y = 16;
    player.direction = 1; // Down
    player.lives = 3;
    player.score = 0;

    // Create Pacman shapes
    sfCircleShape *pacmanBody = sfCircleShape_create();
    sfCircleShape_setRadius(pacmanBody, (float)(SCALE / 2));
    sfCircleShape_setFillColor(pacmanBody, sfYellow);
    sfConvexShape *pacmanMouth = sfConvexShape_create();
    sfConvexShape_setPointCount(pacmanMouth, 3);
    sfConvexShape_setPoint(pacmanMouth, 0, (sfVector2f){0, 0});
    sfConvexShape_setPoint(pacmanMouth, 1, (sfVector2f){SCALE / 2, -SCALE / 2});
    sfConvexShape_setPoint(pacmanMouth, 2, (sfVector2f){SCALE / 2, SCALE / 2});
    sfConvexShape_setFillColor(pacmanMouth, sfBlack);

    // Initialize ghosts
    struct Ghost ghosts[4] = {
        {9, 9, 0, 0, 'r'},
        {8, 10, 0, 0, 'b'},
        {9, 10, 0, 0, 'p'},
        {10, 10, 0, 0, 'o'}
    };

    // Create ghost shapes
    sfConvexShape *ghostBody = sfConvexShape_create();
    sfConvexShape_setPointCount(ghostBody, 9);
    sfConvexShape_setPoint(ghostBody, 0, (sfVector2f){SCALE / 4, 0});
    sfConvexShape_setPoint(ghostBody, 1, (sfVector2f){SCALE * 3 / 4, 0});
    sfConvexShape_setPoint(ghostBody, 2, (sfVector2f){SCALE, SCALE / 4});
    sfConvexShape_setPoint(ghostBody, 3, (sfVector2f){SCALE, SCALE});
    sfConvexShape_setPoint(ghostBody, 4, (sfVector2f){SCALE * 3 / 4, SCALE * 3 / 4});
    sfConvexShape_setPoint(ghostBody, 5, (sfVector2f){SCALE /2, SCALE});
    sfConvexShape_setPoint(ghostBody, 6, (sfVector2f){SCALE / 4, SCALE * 3 / 4});
    sfConvexShape_setPoint(ghostBody, 7, (sfVector2f){0, SCALE});
    sfConvexShape_setPoint(ghostBody, 8, (sfVector2f){0, SCALE / 4});
    sfCircleShape *ghostEye = sfCircleShape_create();
    sfCircleShape_setRadius(ghostEye, (float)(SCALE / 4));
    sfCircleShape *ghostPupil = sfCircleShape_create();
    sfCircleShape_setRadius(ghostPupil, (float)(SCALE / 8));
    sfCircleShape_setFillColor(ghostPupil, sfBlack);

    // Initialize pellet counter
    int pelletCount = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (map[i][j] == 2 || map[i][j] == 3) {
                pelletCount++;
            }
        }
    }

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }

            //input handling
            if (event.type == sfEvtKeyPressed) {
                switch (event.key.code) {
                    case sfKeyUp:
                        player.direction = 3;
                        break;
                    case sfKeyDown:
                        player.direction = 1;
                        break;
                    case sfKeyLeft:
                        player.direction = 2;
                        break;
                    case sfKeyRight:
                        player.direction = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        sfTime time = sfClock_getElapsedTime(clock);
        float seconds = sfTime_asSeconds(time);
        if (seconds >= 0.1) {
            // Move Pacman based on direction
            int nextX = player.x;
            int nextY = player.y;

            // Determine next position based on direction
            switch (player.direction) {
                case 0:  // Right
                    if (player.x == WIDTH - 1) {nextX = 0;}
                    else {nextX++;}
                    break;
                case 1:  // Down
                    nextY++;
                    break;
                case 2:  // Left
                    if (player.x == 0) {nextX = WIDTH - 1;}
                    else {nextX--;}
                    break;
                case 3:  // Up
                    nextY--;
                    break;
            }

            // Check if next position is valid (not a wall) before moving Pacman there
            if (nextX >= 0 && nextX < WIDTH && nextY >= 0 && nextY < HEIGHT && map[nextY][nextX] != 4) {
                player.x = nextX;
                player.y = nextY;
            }

            // Check if pac-man is on a ghost
            for (int i = 0; i < sizeof(ghosts) / sizeof(ghosts[0]); i++) {
                if (player.x == ghosts[i].x && player.y == ghosts[i].y) {
                    player.lives -= 1;
                    player.x = 9;
                    player.y = 16;
                    player.direction = 1;
                }
            }

            // Move Ghosts
            for (int i = 0; i < sizeof(ghosts) / sizeof(ghosts[0]); i++) {
                struct Ghost *ghost;
                ghost = &ghosts[i];
                int validDirs[3] = {0, 0, 0}; // -1 = left, 0 = straight, 1 = right
                int numValidDirs = 0;

                // Populate validDirs
                for (int i = 0; i < sizeof(validDirs) / sizeof(validDirs[0]); i++) {
                    if (map[getNextY(rotate(ghost->direction, i - 1), ghost->y)][getNextX(rotate(ghost->direction, i - 1), ghost->x)] != 4) {
                        validDirs[numValidDirs] = i - 1;
                        numValidDirs++;
                    }
                }

                // If left and right side have walls
                if (map[getNextY(rotate(ghost->direction, 1), ghost->y)][getNextX(rotate(ghost->direction, 1), ghost->x)] == 4 &&
                    map[getNextY(rotate(ghost->direction, -1), ghost->y)][getNextX(rotate(ghost->direction, -1), ghost->x)] == 4) {
                    
                    // If front has a wall
                    if (map[getNextY(ghost->direction, ghost->y)][getNextX(ghost->direction, ghost->x)] == 4) {

                        // Change direction to back
                        ghost->direction = rotate(ghost->direction, 2);
                    }
                }

                // If at an intersection
                else {
                    // Pick random valid direction
                    ghost->direction = rotate(ghost->direction, validDirs[rand() % numValidDirs]);
                }

                // Move ghost in direction
                ghost->x = getNextX(ghost->direction, ghost->x);
                ghost->y = getNextY(ghost->direction, ghost->y);
                
                // Check if pac-man is on a ghost
                for (int i = 0; i < sizeof(ghosts) / sizeof(ghosts[0]); i++) {
                    if (player.x == ghosts[i].x && player.y == ghosts[i].y) {
                        player.lives -= 1;
                        player.x = 9;
                        player.y = 16;
                        player.direction = 1;
                    }
                }
            }

            // Check if pac-man is on a pellet or powerup
            if (map[player.y][player.x] == 2) {
                player.score += 10;
                pelletCount--;
                map[player.y][player.x] = 1;
            }
            else if (map[player.y][player.x] == 3) {
                player.score += 50;
                pelletCount--;
                map[player.y][player.x] = 1;
            }

            sfClock_restart(clock);
        }
            
        // Clear window
        sfRenderWindow_clear(window, sfBlack);
        
        //Draw Map
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int value = map[y][x];
                if (value == 4) {
                    sfRectangleShape_setPosition(wall, (sfVector2f){x * SCALE, y * SCALE});
                    sfRenderWindow_drawRectangleShape(window, wall, NULL);
                }
                else if (value == 3) {
                    sfCircleShape_setPosition(powerup, (sfVector2f){x * SCALE + SCALE / 2 - sfCircleShape_getRadius(powerup), y * SCALE + SCALE / 2 - sfCircleShape_getRadius(powerup)});
                    sfRenderWindow_drawCircleShape(window, powerup, NULL);
                }
                else if (value == 2) {
                    sfCircleShape_setPosition(pellet, (sfVector2f){x * SCALE + SCALE / 2 - sfCircleShape_getRadius(pellet), y * SCALE + SCALE / 2 - sfCircleShape_getRadius(pellet)});
                    sfRenderWindow_drawCircleShape(window, pellet, NULL);
                }
            }
        }

        //Draw Pac Man
        sfCircleShape_setPosition(pacmanBody, (sfVector2f){player.x * SCALE, player.y * SCALE});
        sfRenderWindow_drawCircleShape(window, pacmanBody, NULL);
        sfConvexShape_setPosition(pacmanMouth, (sfVector2f){player.x * SCALE + SCALE / 2, player.y * SCALE + SCALE / 2});
        sfConvexShape_setRotation(pacmanMouth, 90 * player.direction);
        if (player.x % 2 == player.y % 2) {sfRenderWindow_drawConvexShape(window, pacmanMouth, NULL);}

        //Draw Ghosts
        for (int i = 0; i < sizeof(ghosts) / sizeof(ghosts[0]); i++) {
            switch (ghosts[i].color) {
            case 'r':
                sfConvexShape_setFillColor(ghostBody, sfRed);
                break;
            case 'b':
                sfConvexShape_setFillColor(ghostBody, (sfColor){144, 213, 255, 255});
                break;
            case 'p':
                sfConvexShape_setFillColor(ghostBody, (sfColor){255, 182, 193, 255});
                break;
            case 'o':
                sfConvexShape_setFillColor(ghostBody, (sfColor){255, 165, 0, 255});
                break;
            default:
                break;
            }
            sfConvexShape_setPosition(ghostBody, (sfVector2f){ghosts[i].x * SCALE, ghosts[i].y * SCALE});
            sfRenderWindow_drawConvexShape(window, ghostBody, NULL);
            sfCircleShape_setPosition(ghostEye, (sfVector2f){ghosts[i].x * SCALE, ghosts[i].y * SCALE + SCALE / 6});
            sfRenderWindow_drawCircleShape(window, ghostEye, NULL);
            sfCircleShape_setPosition(ghostEye, (sfVector2f){ghosts[i].x * SCALE + SCALE / 2, ghosts[i].y * SCALE + SCALE / 6});
            sfRenderWindow_drawCircleShape(window, ghostEye, NULL);
            sfVector2f leftEyeDirection;
            sfVector2f rightEyeDirection;
            switch(ghosts[i].direction) {
                case 0:
                    leftEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.3, ghosts[i].y * SCALE + SCALE * 0.3};
                    rightEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.8, ghosts[i].y * SCALE + SCALE * 0.3};
                    break;
                case 1:
                    leftEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.15, ghosts[i].y * SCALE + SCALE * 0.45};
                    rightEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.65, ghosts[i].y * SCALE + SCALE * 0.45};
                    break;
                case 2:
                    leftEyeDirection = (sfVector2f){ghosts[i].x * SCALE, ghosts[i].y * SCALE + SCALE * 0.3};
                    rightEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.5, ghosts[i].y * SCALE + SCALE * 0.3};
                    break;
                default:
                    leftEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.15, ghosts[i].y * SCALE + SCALE * 0.15};
                    rightEyeDirection = (sfVector2f){ghosts[i].x * SCALE + SCALE * 0.65, ghosts[i].y * SCALE + SCALE * 0.15};
                    break;
            }
            sfCircleShape_setPosition(ghostPupil, leftEyeDirection);
            sfRenderWindow_drawCircleShape(window, ghostPupil, NULL);
            sfCircleShape_setPosition(ghostPupil, rightEyeDirection);
            sfRenderWindow_drawCircleShape(window, ghostPupil, NULL);

        }

        // Update window
        sfRenderWindow_display(window);

        // Check for game end
        if (pelletCount == 0) {
            sfRenderWindow_close(window);
        }
    }
    return 0;
}