#ifndef main_H
#define main_H

#include <pebble.h>

#define LIST_MESSAGE_WINDOW_NUM_ROWS    5
#define LIST_MESSAGE_WINDOW_CELL_HEIGHT 25
#define LIST_MESSAGE_WINDOW_MENU_HEIGHT \
    LIST_MESSAGE_WINDOW_NUM_ROWS * LIST_MESSAGE_WINDOW_CELL_HEIGHT
#define LIST_MESSAGE_WINDOW_HINT_TEXT   "Select an Exercise"

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
static void select_click_handler(ClickRecognizerRef recognize, void *context);
static void click_config_provider(void *context);
static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context);
static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context);
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context);
static void window_load(Window *window);
static void window_unload(Window *window);
static void init(void);

#endif
