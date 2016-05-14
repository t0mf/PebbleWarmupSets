#ifndef timer_H
#define timer_H

#include <pebble.h>

Window *s_timer_window;

static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed);
void timer_callback(void);
static void timer_select_click_handler(ClickRecognizerRef recognize, void *context);
static void timer_down_click_handler(ClickRecognizerRef recognize, void *context);
static void timer_up_click_handler(ClickRecognizerRef recognize, void *context);
void stop_timer(void);
static void check_time(void);
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed);
static void timer_click_config_provider(void *context);
static void timer_window_load(Window *window);
static void timer_window_unload(Window *window);
void init_timer_window(void);
#endif