#include <pebble.h>
#include "sets.h"
#include "master.h"
#include "mini-printf.h"

static MenuLayer *s_menu_layer;
static TextLayer *s_list_message_layer;
static TextLayer *s_list_message_layer2;

char *sets[] = {"2x5 ","1x5 ","1x3 ","1x2 ","3x5 "};
int weights[] = {123, 123, 123, 123, 123};
int LIST_MESSAGE_WINDOW_NUM_ROWS = 5;

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {

}

static void select_click_handler(ClickRecognizerRef recognize, void *context) {
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return LIST_MESSAGE_WINDOW_NUM_ROWS;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  if (strcmp(exercise_string,"Deadlift") == 0 && cell_index->row == 4) { }
  else {
    static char s_buff[16];
    snprintf(s_buff, sizeof(s_buff), "%s %d %s", sets[cell_index->row], weights[cell_index->row], " lbs");
    menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);
  }
  
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    LIST_MESSAGE_WINDOW_CELL_HEIGHT);
}

static void sets_window_load(Window *window) {
  calculate_sets();
  calculate_weights();
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, LIST_MESSAGE_WINDOW_MENU_HEIGHT));
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_click
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
  
  const GEdgeInsets message_insets = {.top = 140};
  s_list_message_layer = text_layer_create(grect_inset(bounds, message_insets));
  text_layer_set_text_alignment(s_list_message_layer, GTextAlignmentCenter);
  text_layer_set_text(s_list_message_layer, exercise_string);
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer));
  
  const GEdgeInsets message_insets2 = {.top = 155};
  s_list_message_layer2 = text_layer_create(grect_inset(bounds, message_insets2));
  text_layer_set_text_alignment(s_list_message_layer2, GTextAlignmentCenter);
  text_layer_set_text(s_list_message_layer2, m_weight_c);
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer2));
}

static void sets_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
  text_layer_destroy(s_list_message_layer);
}

void init_sets_window(void) {
  if(!s_sets_window) {
    s_sets_window = window_create();
    window_set_click_config_provider(s_sets_window, click_config_provider);
    window_set_window_handlers(s_sets_window, (WindowHandlers) {
        .load = sets_window_load,
        .unload = sets_window_unload,
    });
  }
  
  window_stack_push(s_sets_window, true);  
}

static void deinit(void){
    window_destroy(s_sets_window);
}

void calculate_sets(void) {
  if (strcmp(exercise_string,"Deadlift") == 0) {
    sets[1] = "1x3 ";
    sets[2] = "1x2 ";
    sets[3] = "1x5 ";
    LIST_MESSAGE_WINDOW_NUM_ROWS = 4;
  } else if (strcmp(exercise_string,"Power Cleans") == 0) {
    sets[4] = "5x3 ";
  } else {
    LIST_MESSAGE_WINDOW_NUM_ROWS = 5;
  }
}

void calculate_weights(void) {
  if (strcmp(exercise_string,"Squat") == 0) {
    weights[0] = findWeight(0.0 * m_weight_i);
    weights[1] = findWeight(0.4 * m_weight_i);
    weights[2] = findWeight(0.6 * m_weight_i);
    weights[3] = findWeight(0.8 * m_weight_i);
    weights[4] = findWeight(1.0 * m_weight_i);
  }
  else if (strcmp(exercise_string,"Bench Press") == 0) {
    weights[0] = findWeight(0.0 * m_weight_i);
    weights[1] = findWeight(0.5 * m_weight_i);
    weights[2] = findWeight(0.7 * m_weight_i);
    weights[3] = findWeight(0.9 * m_weight_i);
    weights[4] = findWeight(1.0 * m_weight_i);
  }
  else if (strcmp(exercise_string,"Deadlift") == 0) {
    weights[0] = findWeight(0.4 * m_weight_i);
    weights[1] = findWeight(0.6 * m_weight_i);
    weights[2] = findWeight(0.85 * m_weight_i);
    weights[3] = findWeight(1.0 * m_weight_i);
  }
  else if (strcmp(exercise_string,"Overhead Press") == 0) {
    weights[0] = findWeight(0.0 * m_weight_i);
    weights[1] = findWeight(0.55 * m_weight_i);
    weights[2] = findWeight(0.7 * m_weight_i);
    weights[3] = findWeight(0.85 * m_weight_i);
    weights[4] = findWeight(1.0 * m_weight_i);
  }
  else if (strcmp(exercise_string,"Pendlay Row") == 0) {
    weights[0] = findWeight(0.0 * m_weight_i);
    weights[1] = findWeight(0.5 * m_weight_i);
    weights[2] = findWeight(0.7 * m_weight_i);
    weights[3] = findWeight(0.9 * m_weight_i);
    weights[4] = findWeight(1.0 * m_weight_i);
  }
  else if (strcmp(exercise_string,"Power Cleans") == 0) {
    weights[0] = findWeight(0.0 * m_weight_i);
    weights[1] = findWeight(0.55 * m_weight_i);
    weights[2] = findWeight(0.7 * m_weight_i);
    weights[3] = findWeight(0.85 * m_weight_i);
    weights[4] = findWeight(1.0 * m_weight_i);
  }
}


int findWeight(int num) {
  if ((num - (num % 5)) < 45)
    return 45;
  else
    return (num - (num % 5));
}


