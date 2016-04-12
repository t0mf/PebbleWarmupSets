#ifndef timer_H
#define timer_H

#include <pebble.h>

Window *s_timer_window;

static void timer_callback(void *data);
static void timer_window_load(Window *window);
static void timer_window_unload(Window *window);
void init_timer_window(void);
static void deinit(void);

#endif