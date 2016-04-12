#include "main.h"
#include "weight.h"
#include "master.h"

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static TextLayer *s_list_message_layer;

// Initialize exercise names
char *exercise_name_strings[6] = {"Squat", "Bench Press","Deadlift","Overhead Press","Pendlay Row","Power Cleans"};
char *exercise_set_strings[6][5] = { 
    {"2x5","1x5","1x3","1x2","3x5"}, // Squat
    {"2x5","1x5","1x3","1x2","3x5"}, // Bench Press
    {"2x5","1x3","1x2","1x5",""},    // Deadlift
    {"2x5","1x5","1x3","1x2","3x5"}, // Overhead Press
    {"2x5","1x5","1x3","1x2","3x5"}, // Pendlay Rows
    {"2x5","1x5","1x3","1x2","5x3"}  // Power Cleans
  };

double exercise_multipliers[6][5] = {
  {0.0,0.4,0.6,0.8,1.0},  // Squat
  {0.0,0.5,0.7,0.9,1.0},  // Bench Press
  {0.4,0.6,0.85,1.0},     // Deadlift
  {0.0,0.55,0.7,0.85,1.0},// Overhead Press
  {0.0,0.5,0.7,0.9,1.0},  // Pendlay Rows
  {0.0,0.55,0.7,0.85,1.0} // Power Cleans
};

// Initialize plate weights for imperial and metric
  double plate_weights[6][2] = { {45,20}, {35,15}, {25,10}, {10,5}, {5,2.5}, {2.5,1.25} };
  // Initialize step size weights for imperial and metric
  double step_size[2] = {5, 2.5};
  // Initialize barbell weights for imperial and metric
  int bar_type = 0; //0 for 45, 1 for 35, 2 for 25
  double barbell_weights[3][2] = { {45,20}, {35,15}, {25, 10} };

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  exercise_int = cell_index->row;  
  init_weight_window();
  
  window_stack_push(s_weight_window, true);
}

static void select_click_handler(ClickRecognizerRef recognize, void *context) {
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return 6;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  static char s_buff[16];
  snprintf(s_buff, sizeof(s_buff), "%s", exercise_name_strings[cell_index->row]);
  menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    25);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 150));
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
  text_layer_set_text(s_list_message_layer, "Select an Exercise");
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
  text_layer_destroy(s_list_message_layer);
}

static void init(void) {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_click_config_provider(s_main_window, click_config_provider);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  
  window_stack_push(s_main_window, true);  
}

static void deinit(void){
    window_destroy(s_main_window);
}

int main() {
  
  // Initialize imperial or metric
  unit_system = 0;
  // Initialize kg or lbs
  if (unit_system == 0) {
    unit_type = " lbs";
    m_weight_i = 45;
  } else if (unit_system == 1) {
    unit_type = " kgs";
    m_weight_i = 20;
  }
  
  
  init();

  app_event_loop();

  deinit();
}