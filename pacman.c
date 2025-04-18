#include <stdio.h>
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

int main() {
    //Create window
    sfVideoMode mode = {WIDTH * SCALE, HEIGHT * SCALE, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Pac Man", sfResize | sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);

    //Create clock
    sfClock *clock = sfClock_create();

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
    player.direction = 0; // Right
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

            // Check if pac-man is on a pellet or powerup, and eat it
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
        for (int i = 0; i < 4; i++) {
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
        }

        sfRenderWindow_display(window);

        // Check for game end
        if (pelletCount == 0) {
            sfRenderWindow_close(window);
        }
    }
    return 0;
}