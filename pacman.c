#include <stdio.h>
#include <SFML/Graphics.h>


const int HEIGHT = 22;
const int WIDTH = 19;
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
    sfVideoMode mode = {WIDTH * SCALE, HEIGHT * SCALE, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Pac Man", sfResize | sfClose, NULL);

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

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
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
        sfRenderWindow_display(window);
    }
    return 0;
}