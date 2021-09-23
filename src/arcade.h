#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cglm/cglm.h>

void arcade_window_create(uint32_t width, uint32_t height, const char* title);

void arcade_set_clear_color(vec4 color);

bool arcade_run(void);

void arcade_begin_render(void);

void arcade_window_destroy(void);
