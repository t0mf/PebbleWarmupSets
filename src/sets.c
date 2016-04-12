#include <pebble.h>
#include <math.h>
#include "sets.h"
#include "master.h"
#include "mini-printf.h"
#include "ftoa.h"

static MenuLayer *s_menu_layer;
static TextLayer *s_list_message_layer;
static TextLayer *s_list_message_layer2;

char *sets[] = {"2x5 ","1x5 ","1x3 ","1x2 ","3x5 "};
double weights[] = {123.0, 123.0, 123.0, 123.0, 123.0};
char plates[5][N+1];

char s_buff[10];

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {

}

static void select_click_handler(ClickRecognizerRef recognize, void *context) {
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  if (strcmp(exercise_name_strings[exercise_int],"Deadlift") == 0) { return 4; } else { return 5; }
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  if (strcmp(exercise_name_strings[exercise_int],"Deadlift") == 0 && cell_index->row == 4) { }
  else {
    static char s_buff4[16];
    ftoa(s_buff, weights[cell_index->row], 5);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", s_buff);
    snprintf(s_buff4, sizeof(s_buff4), "%s %s %s", sets[cell_index->row], s_buff, unit_type);
    
    menu_cell_basic_draw(ctx, cell_layer, s_buff4, plates[cell_index->row], NULL);
  }
  
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    25);
}

static void sets_window_load(Window *window) {
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_menu_layer = menu_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 125));
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
  text_layer_set_text(s_list_message_layer, exercise_name_strings[exercise_int]);
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
  calculate_sets();
  calculate_weights();
  do_stuff();
  window_stack_push(s_sets_window, true);  
}

static void deinit(void){
    window_destroy(s_sets_window);
}

void do_stuff(void) {
    for (int i = 0; i < 5; i++) {
      calculate_barbell_math(weights[i], i);
    }
}

void calculate_sets(void) {
  for (int i = 0; i < 5; i++) {
    sets[i] = exercise_set_strings[exercise_int][i];
  }
}

void calculate_weights(void) {
  if (strcmp(exercise_name_strings[exercise_int],"Deadlift") == 0) {
    for (int i = 0; i < 4; i++) {
      weights[i] = findWeight(exercise_multipliers[exercise_int][i] *m_weight_i);
    }
  } else {
    for (int i = 0; i < 5; i++) {
      weights[i] = findWeight(exercise_multipliers[exercise_int][i] *m_weight_i);
    }
  }
}

double findWeight(double num) {
  int tmp = num;
  if (unit_system == 0) {
    if ((tmp - (tmp % 5)) < 45)
      return 45;
    else
    return (tmp - (tmp % 5));
  } else if (unit_system == 1) {
      num /= 2.5;
      num = floor(num);
      num *= 2.5;

      return num;
      //return 20;
    //else
    //return (num - (num % 3));
  }
  return -1;
}

void calculate_barbell_math(double weight, int i) {

  double bar_weight = barbell_weights[bar_type][unit_system];
  
  weight -= bar_weight;
  
  if ( weight == 0) { strcpy(plates[i],"Bar"); }
  
  weight /= 2;
  
  int weight_45 = 0;
  int weight_35 = 0;
  int weight_25 = 0;
  int weight_10 = 0;
  int weight_5 = 0;
  int weight_2half = 0;
    
  while (weight > 0) {
    if (weight < plate_weights[4][unit_system]) {
      weight_2half += 1;
      weight -= plate_weights[5][unit_system];
    } else if (weight < plate_weights[3][unit_system]) {
      weight_5 += 1;
      weight -= plate_weights[4][unit_system];
    } else if (weight < plate_weights[2][unit_system]) {
      weight_10 += 1;
      weight -= plate_weights[3][unit_system];
    } else if (weight < plate_weights[1][unit_system]) {
      weight_25 += 1;
      weight -= plate_weights[2][unit_system];
    } else if (weight < plate_weights[0][unit_system]) {
      weight_35 += 1;
      weight -= plate_weights[1][unit_system];
    } else if (weight >= plate_weights[0][unit_system]) {
      weight_45 += 1;
      weight -= plate_weights[0][unit_system];
    }  
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "%d %d %d %d %d %d", weight_45, weight_35, weight_25, weight_10, weight_5, weight_2half);

  int head = 0, ret;
  
  if (weight_45 != 0) {
    ftoa(s_buff, plate_weights[0][unit_system], 2);
    if (weight_45 > 1) {
      ret = snprintf(plates[i] + head, 10, "%d %s %s %s", weight_45, "x", s_buff, " ");
      head += ret;
    } else {
      ret = snprintf(plates[i] + head, 10, "%s %s", s_buff, " ");
      head += ret;
    }
  }
  if (weight_35 != 0) {
    ftoa(s_buff, plate_weights[1][unit_system], 2);
    if (weight_35 > 1) {
      ret = snprintf(plates[i] + head, 10, "%d %s %s %s", weight_35, "x", s_buff, " ");
      head += ret;
    } else {
      ret = snprintf(plates[i] + head, 5, "%s %s", s_buff, " ");
      head += ret;
    }
  }
  if (weight_25 != 0) {
    ftoa(s_buff, plate_weights[2][unit_system], 2);
    if (weight_25 > 1) {
      ret = snprintf(plates[i] + head, 10, "%d %s %s %s", weight_25, "x", s_buff, " ");
      head += ret;
    } else {
      ret = snprintf(plates[i] + head, 5, "%s %s", s_buff, " ");
      head += ret;
    }
  }
  if (weight_10 != 0) {
    ftoa(s_buff, plate_weights[3][unit_system], 2);
    if (weight_10 > 1) {
      ret = snprintf(plates[i] + head, 10, "%d %s %s %s", weight_10, "x", s_buff, " ");
      head += ret;
    } else { 
      ret = snprintf(plates[i] + head, 5, "%s %s", s_buff, " ");
      head += ret;
    }
  }
  if (weight_5 != 0) {
    ftoa(s_buff, plate_weights[4][unit_system], 2);
    if (weight_5 > 1) {
      ret = snprintf(plates[i] + head, 10, "%d %s %s %s", weight_5, "x", s_buff, " ");
    } else {
      ret = snprintf(plates[i] + head, 5, "%s %s", s_buff, " ");
      head += ret;
    }
  }
  if (weight_2half != 0) {
    ftoa(s_buff, plate_weights[5][unit_system], 2);
    if (weight_2half > 1) {
      ret = snprintf(plates[i] + head, 10, "%d %s %s %s", weight_2half, "x", s_buff, " ");
      head += ret;
    } else {
      ret = snprintf(plates[i] + head, 5, "%s %s", s_buff, " ");
      head += ret;
    }
  }
   APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", plates[i]);
}


















