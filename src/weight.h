#ifndef weight_H
#define weight_H

#include <pebble.h>

static void weight_select_click_handler(ClickRecognizerRef recognize, void *context);
static void weight_down_click_handler(ClickRecognizerRef recognize, void *context);
static void weight_up_click_handler(ClickRecognizerRef recognize, void *context);
static void weight_window_load(Window *window);
static void weight_window_unload(Window *window);
void init_weight_window(void);
Window *s_weight_window;

#endif