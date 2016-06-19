#include "main.h"
#include "weight.h"
#include "master.h"

#define KEY_UNIT_TYPE 1000
#define KEY_BAR_WEIGHT 2000
#define KEY_SQUAT_1 3000
#define KEY_SQUAT_2 4000
#define KEY_SQUAT_3 5000
#define KEY_SQUAT_4 6000
#define KEY_BENCH_1 7000
#define KEY_BENCH_2 8000
#define KEY_BENCH_3 9000
#define KEY_BENCH_4 10000
#define KEY_DEAD_1 11000
#define KEY_DEAD_2 12000
#define KEY_DEAD_3 13000
#define KEY_PRESS_1 14000
#define KEY_PRESS_2 15000
#define KEY_PRESS_3 16000
#define KEY_PRESS_4 17000
#define KEY_ROW_1 18000
#define KEY_ROW_2 19000
#define KEY_ROW_3 20000
#define KEY_ROW_4 21000
#define KEY_CLEAN_1 22000
#define KEY_CLEAN_2 23000
#define KEY_CLEAN_3 24000
#define KEY_CLEAN_4 25000

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static TextLayer *s_list_message_layer;
static TextLayer *s_time_layer;

// Initialize exercise names
char *exercise_name_strings[6] = {"Squat", "Bench Press","Deadlift","Overhead Press","Pendlay Row","Power Cleans"};
// Initialize sets
char *exercise_set_strings[6][5] = { 
    {"2x5","1x5","1x3","1x2","3x5"}, // Squat
    {"2x5","1x5","1x3","1x2","3x5"}, // Bench Press
    {"2x5","1x3","1x2","1x5",""},    // Deadlift
    {"2x5","1x5","1x3","1x2","3x5"}, // Overhead Press
    {"2x5","1x5","1x3","1x2","3x5"}, // Pendlay Rows
    {"2x5","1x5","1x3","1x2","5x3"}  // Power Cleans
  };
// Initialize multipliers
double exercise_multipliers[6][5] = {
  {0.0,0.4,0.6,0.8,1.0},  // Squat
  {0.0,0.5,0.7,0.9,1.0},  // Bench Press
  {0.4,0.6,0.85,1.0},     // Deadlift
  {0.0,0.55,0.7,0.85,1.0},// Overhead Press
  {0.0,0.5,0.7,0.9,1.0},  // Pendlay Rows
  {0.0,0.55,0.7,0.85,1.0} // Power Cleans
};
// Initialize plate weights for imperial and metric
double plate_weights[6][2] = { {45,20.00}, {35,15.00}, {25,10.00}, {10,5.00}, {5,2.50}, {2.5,1.25} };
// Initialize step size weights for imperial and metric
double step_size[2] = {5, 2.5};
// Initialize barbell weights for imperial and metric
int bar_type = 0; //0 for 45, 1 for 35, 2 for 25
int barbell_weights[2] = { 45,20 };

static char s_time_text[] = "00:00   ";

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *unit_system_t = dict_find(iter, KEY_UNIT_TYPE);

  if (unit_system_t) {
    unit_system = unit_system_t->value->int8;
    persist_write_int(0, unit_system);

    if (unit_system == 0) { unit_type = " lbs"; } 
    else if (unit_system == 1) { unit_type = " kgs"; }
  }

  Tuple *bar_weight_t = dict_find(iter, KEY_BAR_WEIGHT);

  if (bar_weight_t) {
    barbell_weights[unit_system] = bar_weight_t->value->int8;
    persist_write_int(1, bar_weight_t->value->int8);
    m_weight_d = bar_weight = barbell_weights[unit_system];
  }

  Tuple *squat_1_t = dict_find(iter, KEY_SQUAT_1);
  Tuple *squat_2_t = dict_find(iter, KEY_SQUAT_2);
  Tuple *squat_3_t = dict_find(iter, KEY_SQUAT_3);
  Tuple *squat_4_t = dict_find(iter, KEY_SQUAT_4);

  if (squat_1_t) {
    exercise_multipliers[0][0] = (squat_1_t->value->int16)/1000.0;
    exercise_multipliers[0][1] = (squat_2_t->value->int16)/1000.0;
    exercise_multipliers[0][2] = (squat_3_t->value->int16)/1000.0;
    exercise_multipliers[0][3] = (squat_4_t->value->int16)/1000.0;
    persist_write_int(KEY_SQUAT_1, squat_1_t->value->int16);
    persist_write_int(KEY_SQUAT_2, squat_2_t->value->int16);
    persist_write_int(KEY_SQUAT_3, squat_3_t->value->int16);
    persist_write_int(KEY_SQUAT_4, squat_4_t->value->int16);
  }

  Tuple *bench_1_t = dict_find(iter, KEY_BENCH_1);
  Tuple *bench_2_t = dict_find(iter, KEY_BENCH_2);
  Tuple *bench_3_t = dict_find(iter, KEY_BENCH_3);
  Tuple *bench_4_t = dict_find(iter, KEY_BENCH_4);

  if (bench_1_t) {
    exercise_multipliers[1][0] = (bench_1_t->value->int16)/1000.0;
    exercise_multipliers[1][1] = (bench_2_t->value->int16)/1000.0;
    exercise_multipliers[1][2] = (bench_3_t->value->int16)/1000.0;
    exercise_multipliers[1][3] = (bench_4_t->value->int16)/1000.0;
    persist_write_int(KEY_BENCH_1, bench_1_t->value->int16);
    persist_write_int(KEY_BENCH_2, bench_2_t->value->int16);
    persist_write_int(KEY_BENCH_3, bench_3_t->value->int16);
    persist_write_int(KEY_BENCH_4, bench_4_t->value->int16);

  }

  Tuple *dead_1_t = dict_find(iter, KEY_DEAD_1);
  Tuple *dead_2_t = dict_find(iter, KEY_DEAD_2);
  Tuple *dead_3_t = dict_find(iter, KEY_DEAD_3);

  if (dead_1_t) {
    exercise_multipliers[2][0] = (dead_1_t->value->int16)/1000.0;
    exercise_multipliers[2][1] = (dead_2_t->value->int16)/1000.0;
    exercise_multipliers[2][2] = (dead_3_t->value->int16)/1000.0;
    persist_write_int(KEY_DEAD_1, dead_1_t->value->int16);
    persist_write_int(KEY_DEAD_2, dead_2_t->value->int16);
    persist_write_int(KEY_DEAD_3, dead_3_t->value->int16);

  }

  Tuple *press_1_t = dict_find(iter, KEY_PRESS_1);
  Tuple *press_2_t = dict_find(iter, KEY_PRESS_2);
  Tuple *press_3_t = dict_find(iter, KEY_PRESS_3);
  Tuple *press_4_t = dict_find(iter, KEY_PRESS_4);

  if (press_1_t) {
    exercise_multipliers[3][0] = (press_1_t->value->int16)/1000.0;
    exercise_multipliers[3][1] = (press_2_t->value->int16)/1000.0;
    exercise_multipliers[3][2] = (press_3_t->value->int16)/1000.0;
    exercise_multipliers[3][3] = (press_4_t->value->int16)/1000.0;
    persist_write_int(KEY_PRESS_1, press_1_t->value->int16);
    persist_write_int(KEY_PRESS_2, press_2_t->value->int16);
    persist_write_int(KEY_PRESS_3, press_3_t->value->int16);
    persist_write_int(KEY_PRESS_4, press_4_t->value->int16);

  }

  Tuple *row_1_t = dict_find(iter, KEY_ROW_1);
  Tuple *row_2_t = dict_find(iter, KEY_ROW_2);
  Tuple *row_3_t = dict_find(iter, KEY_ROW_3);
  Tuple *row_4_t = dict_find(iter, KEY_ROW_4);

  if (row_1_t) {
    exercise_multipliers[4][0] = (row_1_t->value->int16)/1000.0;
    exercise_multipliers[4][1] = (row_2_t->value->int16)/1000.0;
    exercise_multipliers[4][2] = (row_3_t->value->int16)/1000.0;
    exercise_multipliers[4][3] = (row_4_t->value->int16)/1000.0;
    persist_write_int(KEY_ROW_1, row_1_t->value->int16);
    persist_write_int(KEY_ROW_2, row_2_t->value->int16);
    persist_write_int(KEY_ROW_3, row_3_t->value->int16);
    persist_write_int(KEY_ROW_4, row_4_t->value->int16);

  }

  Tuple *clean_1_t = dict_find(iter, KEY_CLEAN_1);
  Tuple *clean_2_t = dict_find(iter, KEY_CLEAN_2);
  Tuple *clean_3_t = dict_find(iter, KEY_CLEAN_3);
  Tuple *clean_4_t = dict_find(iter, KEY_CLEAN_4);

  if (clean_1_t) {
    exercise_multipliers[5][0] = (clean_1_t->value->int16)/1000.0;
    exercise_multipliers[5][1] = (clean_2_t->value->int16)/1000.0;
    exercise_multipliers[5][2] = (clean_3_t->value->int16)/1000.0;
    exercise_multipliers[5][3] = (clean_4_t->value->int16)/1000.0;
    persist_write_int(KEY_CLEAN_1, clean_1_t->value->int16);
    persist_write_int(KEY_CLEAN_2, clean_2_t->value->int16);
    persist_write_int(KEY_CLEAN_3, clean_3_t->value->int16);
    persist_write_int(KEY_CLEAN_4, clean_4_t->value->int16);

  }
}

// Method handles changing the clock every minute
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) { // Update the time every minute
      clock_copy_time_string(s_time_text, sizeof(s_time_text));
      text_layer_set_text(s_time_layer,s_time_text);
  }
}

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
  // Draw the rows
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
  // Initialize the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create the menu
  s_menu_layer = menu_layer_create(GRect(bounds.origin.x, bounds.origin.y+20, bounds.size.w, 140));
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_click
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
  
  // Create a "Select an Exercise" text layer
  const GEdgeInsets message_insets = {.top = 140};
  s_list_message_layer = text_layer_create(grect_inset(bounds, message_insets));
  text_layer_set_text_alignment(s_list_message_layer, GTextAlignmentCenter);
  text_layer_set_text(s_list_message_layer, "Select an Exercise");
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer));
  
  // Create the time text layer up top
  s_time_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-30, bounds.origin.y+5}, .size = { 60, 15 } });
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void window_appear(Window *window) {
  clock_copy_time_string(s_time_text, sizeof(s_time_text));
  text_layer_set_text(s_time_layer,s_time_text);
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
  text_layer_destroy(s_list_message_layer);
  text_layer_destroy(s_time_layer);
}

static void init(void) {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_click_config_provider(s_main_window, click_config_provider);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .appear = window_appear,
        .unload = window_unload,
    });
  }
  
  window_stack_push(s_main_window, true);  

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit(void){
    window_destroy(s_main_window);
}

int main() {
  // Initialize imperial or metric system
  if (persist_exists(0)) {
    unit_system = persist_read_int(0);
  } else {
    unit_system = 0;
  }

  if (persist_exists(1)) {
    m_weight_d = bar_weight = barbell_weights[unit_system] = persist_read_int(1);
  } else {
    if (unit_system == 0) {
      m_weight_d = bar_weight = barbell_weights[unit_system] = 45;
    } else if (unit_system == 1) {
      m_weight_d = bar_weight = barbell_weights[unit_system] = 20;
    }
  }
  
  if (persist_exists(KEY_SQUAT_1)) {
    exercise_multipliers[0][0] = persist_read_int(KEY_SQUAT_1)/1000.0;
    exercise_multipliers[0][1] = persist_read_int(KEY_SQUAT_2)/1000.0;
    exercise_multipliers[0][2] = persist_read_int(KEY_SQUAT_3)/1000.0;
    exercise_multipliers[0][3] = persist_read_int(KEY_SQUAT_4)/1000.0;
  }

  if (persist_exists(KEY_BENCH_1)) {
    exercise_multipliers[1][0] = persist_read_int(KEY_BENCH_1)/1000.0;
    exercise_multipliers[1][1] = persist_read_int(KEY_BENCH_2)/1000.0;
    exercise_multipliers[1][2] = persist_read_int(KEY_BENCH_3)/1000.0;
    exercise_multipliers[1][3] = persist_read_int(KEY_BENCH_4)/1000.0;
  }

  if (persist_exists(KEY_DEAD_1)) {
    exercise_multipliers[2][0] = persist_read_int(KEY_DEAD_1)/1000.0;
    exercise_multipliers[2][1] = persist_read_int(KEY_DEAD_2)/1000.0;
    exercise_multipliers[2][2] = persist_read_int(KEY_DEAD_3)/1000.0;
  }

  if (persist_exists(KEY_PRESS_1)) {
    exercise_multipliers[3][0] = persist_read_int(KEY_PRESS_1)/1000.0;
    exercise_multipliers[3][1] = persist_read_int(KEY_PRESS_2)/1000.0;
    exercise_multipliers[3][2] = persist_read_int(KEY_PRESS_3)/1000.0;
    exercise_multipliers[3][3] = persist_read_int(KEY_PRESS_4)/1000.0;
  }

  if (persist_exists(KEY_ROW_1)) {
    exercise_multipliers[4][0] = persist_read_int(KEY_ROW_1)/1000.0;
    exercise_multipliers[4][1] = persist_read_int(KEY_ROW_2)/1000.0;
    exercise_multipliers[4][2] = persist_read_int(KEY_ROW_3)/1000.0;
    exercise_multipliers[4][3] = persist_read_int(KEY_ROW_4)/1000.0;
  }

  if (persist_exists(KEY_CLEAN_1)) {
    exercise_multipliers[5][0] = persist_read_int(KEY_CLEAN_1)/1000.0;
    exercise_multipliers[5][1] = persist_read_int(KEY_CLEAN_2)/1000.0;
    exercise_multipliers[5][2] = persist_read_int(KEY_CLEAN_3)/1000.0;
    exercise_multipliers[5][3] = persist_read_int(KEY_CLEAN_4)/1000.0;
  }

  // Initialize barbell weight
   m_weight_d = bar_weight = barbell_weights[unit_system]; 
  
  // Initialize kg or lbs
  if (unit_system == 0) { unit_type = " lbs"; } 
  else if (unit_system == 1) { unit_type = " kgs"; }
  
  init();

  app_event_loop();

  deinit();
}