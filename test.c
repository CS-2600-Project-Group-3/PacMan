#include <SFML/Window.h>
#include <SFML/Graphics.h>

int main() {
    sfVideoMode mode = {640, 640, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "csfml quick test", sfResize | sfClose, NULL);
    sfCircleShape *circle = sfCircleShape_create();
    sfCircleShape_setRadius(circle, 320.0f);
    sfCircleShape_setFillColor(circle, sfRed);
    sfCircleShape_setOutlineColor(circle, sfWhite);
    sfCircleShape_setOutlineThickness(circle, 1.0f);
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawCircleShape(window, circle, NULL);
        sfRenderWindow_display(window);
    }
    return 0;
} 