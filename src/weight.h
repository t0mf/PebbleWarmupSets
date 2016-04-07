#ifndef weight_H
#define weight_H

#include <pebble.h>

static void weight_select_click_handler(ClickRecognizerRef recognize, void *context);
static void weight_down_click_handler(ClickRecognizerRef recognize, void *context);
static void weight_up_click_handler(ClickRecognizerRef recognize, void *context);
static void weight_click_config_provider(void *context);
static void rep_window_load(Window *window);
static void rep_window_unload(Window *window);
void init_rep_window(void);
Window *s_rep_window;
char *exercise_string;

#endif