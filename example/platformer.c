#include <arcade.h>

#include <stdlib.h>

int main(void)
{
    uint32_t width = 1280;
    uint32_t height = 720;
    const char* title = "Platformer - Arcade";

    arcade_window_create(width, height, title);
    arcade_set_clear_color((vec4) {0.8f, 0.3f, 0.2f, 1.0f});

    while (arcade_run()) {
        arcade_begin_render();
    }

    arcade_window_destroy();

    return EXIT_SUCCESS;
}
