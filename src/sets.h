#ifndef sets_H
#define sets_H

#include <pebble.h>

#define N 100
extern char plates[5][N+1];

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context);
static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context);
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context);
static void sets_window_load(Window *window);
static void sets_window_unload(Window *window);
void init_sets_window(void);
static void deinit(void);
void calculate_sets(void);
void calculate_weights(void);
void calculate_sets(void);
void do_stuff(void);
double findWeight(double num);
void calculate_barbell_math(double weight, int i);
void concatPlates(int plateNumber, int i, double plate_weights);
Window *s_sets_window;
  
#endif