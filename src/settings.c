#include <pebble.h>
#include "main.h"
#include "settings.h"
#include "master.h"

static MenuLayer *settings_menu_layer;
static TextLayer *settings_list_message_layer;
static TextLayer *s_time_layer;

// Initialize exercise names
char *settings[6] = {"Units", "Bar Weight"};

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
      static char s_time_text[] = "00:00   ";
      clock_copy_time_string(s_time_text, sizeof(s_time_text));
      text_layer_set_text(s_time_layer,s_time_text);
  }
}

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  static char s_buff[16];
  snprintf(s_buff, sizeof(s_buff), "%s", settings[cell_index->row]);
  if (cell_index->row == 0) {
    if (unit_system == 1) {
      unit_system = 0;
      unit_type = " lbs";
      m_weight_d = barbell_weights[bar_type][unit_system]; 
    }
    else { unit_system = 1;
      unit_type = " kgs";
      m_weight_d = barbell_weights[bar_type][unit_system]; 

    }
  } else if (cell_index->row == 1) {
    if (bar_type == 0) {
      bar_type = 1;
      m_weight_d = barbell_weights[bar_type][unit_system];
        
    }
    else if (bar_type == 1) {
      bar_type = 2;
      m_weight_d = barbell_weights[bar_type][unit_system];
    
    }
    else if (bar_type == 2) { 
      bar_type = 0; 
      m_weight_d = barbell_weights[bar_type][unit_system];
    
    }
  }
  persist_write_int(0, unit_system);
  persist_write_int(1, bar_type);
  menu_layer_reload_data(settings_menu_layer);
}

static void select_click_handler(ClickRecognizerRef recognize, void *context) {
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return 2;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  static char s_buff[16];
  snprintf(s_buff, sizeof(s_buff), "%s", settings[cell_index->row]);
  if (cell_index->row == 0) {
    menu_cell_basic_draw(ctx, cell_layer, s_buff, unit_type, NULL);
  } else if (cell_index->row == 1) {
    static char s_buff1[16];
    snprintf(s_buff1, sizeof(s_buff1), "%d %s", barbell_weights[bar_type][unit_system], unit_type);
    menu_cell_basic_draw(ctx, cell_layer, s_buff, s_buff1, NULL);
  }
  
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    55);
}

static void settings_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  settings_menu_layer = menu_layer_create(GRect(bounds.origin.x, bounds.origin.y+10, bounds.size.w, 140));
  menu_layer_set_click_config_onto_window(settings_menu_layer, window);
  menu_layer_set_callbacks(settings_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_click
  });
  layer_add_child(window_layer, menu_layer_get_layer(settings_menu_layer));
  
  const GEdgeInsets message_insets = {.top = 140};
  settings_list_message_layer = text_layer_create(grect_inset(bounds, message_insets));
  text_layer_set_text_alignment(settings_list_message_layer, GTextAlignmentCenter);
  text_layer_set_text(settings_list_message_layer, "Settings");
  layer_add_child(window_layer, text_layer_get_layer(settings_list_message_layer));
  
  s_time_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 20));
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  static char s_time_text[] = "00:00   ";
  clock_copy_time_string(s_time_text, sizeof(s_time_text));
  text_layer_set_text(s_time_layer,s_time_text);
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void settings_window_unload(Window *window) {
  window_destroy(s_settings_window);
  s_settings_window = NULL;
}

void init_settings_window(void) {
  if(!s_settings_window) {
    s_settings_window = window_create();
    window_set_click_config_provider(s_settings_window, click_config_provider);
    window_set_window_handlers(s_settings_window, (WindowHandlers) {
        .load = settings_window_load,
        .unload = settings_window_unload,
    });
  }
}