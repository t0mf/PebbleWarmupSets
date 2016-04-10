#ifndef sets_H
#define sets_H

#include <pebble.h>


#define LIST_MESSAGE_WINDOW_CELL_HEIGHT 30
#define LIST_MESSAGE_WINDOW_MENU_HEIGHT \
    5 * LIST_MESSAGE_WINDOW_CELL_HEIGHT
#define LIST_MESSAGE_WINDOW_HINT_TEXT   "Sets"

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
static void select_click_handler(ClickRecognizerRef recognize, void *context);
static void click_config_provider(void *context);
static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context);
static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context);
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context);
static void sets_window_load(Window *window);
static void sets_window_unload(Window *window);
void init_sets_window(void);
static void deinit(void);
void calculate_sets(void);
void calculate_weights(void);
int findWeight(int num);
Window *s_sets_window;

#endif