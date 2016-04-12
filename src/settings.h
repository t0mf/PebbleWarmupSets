#ifndef settings_H
#define settings_H

#include <pebble.h>



static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
static void select_click_handler(ClickRecognizerRef recognize, void *context);
static void click_config_provider(void *context);
static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context);
static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context);
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context);
static void settings_window_load(Window *window);
static void settings_window_unload(Window *window);
void init_settings_window(void);
static void deinit(void);
Window *s_settings_window;
  
#endif