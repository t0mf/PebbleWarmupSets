#include <pebble.h>
#include <stdio.h>
#include "weight.h"
#include "mini-printf.h"
#include "sets.h"
#include "master.h"
#include "ftoa.h"

static TextLayer *s_text_layer;
static TextLayer *s_text_layer2;
static TextLayer *s_plus_minus_layer;
static TextLayer *s_time_layer;
static GBitmap *s_res_up_arrow_button;
static GBitmap *s_res_check_mark_button;
static GBitmap *s_res_down_arrow_button;
static ActionBarLayer *s_actionbarlayer_1;

static int s_max_weight = 500;
char weight_buff[10];
int temp = 0;

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
      static char s_time_text[] = "00:00   ";
      clock_copy_time_string(s_time_text, sizeof(s_time_text));
      text_layer_set_text(s_time_layer,s_time_text);
  }
}

static void weight_select_click_handler(ClickRecognizerRef recognize, void *context) {
  ftoa(weight_buff, m_weight_d, 3);
  text_layer_set_text(s_text_layer2, weight_buff);
  persist_write_int(((exercise_int + 3)+(unit_system*7)), m_weight_d);
  init_sets_window();
  window_stack_push(s_sets_window, true);
}

static void weight_down_click_handler(ClickRecognizerRef recognize, void *context) {
   m_weight_d = m_weight_d - step_size[unit_system];
  
  if (m_weight_d < 0) {
    m_weight_d = s_max_weight;
  }
  ftoa(weight_buff, m_weight_d, 3);
  temp = m_weight_d;
  if (unit_system == 1 && temp % 5 == 0) {
    strcat(weight_buff,".0");
  }
  text_layer_set_text(s_text_layer2, weight_buff);
}

static void weight_up_click_handler(ClickRecognizerRef recognize, void *context) {
  m_weight_d = m_weight_d + step_size[unit_system];

  if (m_weight_d > s_max_weight) {
    m_weight_d = 0;
  }
  
  ftoa(weight_buff, m_weight_d, 2);
  
  temp = m_weight_d;
  if (unit_system == 1 && temp % 5 == 0) {
    strcat(weight_buff,".0");
  }
  text_layer_set_text(s_text_layer2, weight_buff);
  
}

static void weight_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, weight_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, weight_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, weight_up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, weight_up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, weight_down_click_handler);
}

static void weight_window_load(Window *window) {
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-35, bounds.origin.y+20}, .size = { 70, 60 } });
  text_layer_set_text(s_text_layer, "Select a Weight");

  
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));

  if (persist_read_int(((exercise_int + 3)+(unit_system*7))))
  { m_weight_d = persist_read_int(((exercise_int + 3)+(unit_system*7))); } 
 
  
  int tmp = m_weight_d;
  if (unit_system == 1 && (tmp * 2) % 5 == 4) { m_weight_d += .5; }
  
  ftoa(weight_buff, m_weight_d, 3);
  if (unit_system == 1 && tmp % 5 == 0) {
    strcat(weight_buff,".0");
  }
  s_text_layer2 = text_layer_create((GRect) {.origin = {bounds.size.w/2-30, bounds.size.h/2-17}, .size = { 60, 28 } });
  text_layer_set_text_alignment(s_text_layer2, GTextAlignmentCenter);
  text_layer_set_text(s_text_layer2, weight_buff);
  text_layer_set_font(s_text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer2));
  
  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-30, bounds.size.h - 39}, .size = { 60, 60 } });

  text_layer_set_text(s_text_layer, exercise_name_strings[exercise_int]);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  
  s_time_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-30, bounds.origin.y+5}, .size = { 60, 15 } });
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  static char s_time_text[] = "00:00   ";
  clock_copy_time_string(s_time_text, sizeof(s_time_text));
  text_layer_set_text(s_time_layer,s_time_text);
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void weight_window_unload(Window *window) {
  s_weight_window = NULL;
  window_destroy(s_weight_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_up_arrow_button);
  gbitmap_destroy(s_res_check_mark_button);
  gbitmap_destroy(s_res_down_arrow_button);
}

void init_weight_window(void) {
  if (!s_weight_window) {
    s_weight_window = window_create();
    
    #ifndef PBL_SDK_3
      window_set_fullscreen(s_window, true);
    #endif
    if (unit_system == 1) {
      s_res_up_arrow_button = gbitmap_create_with_resource(RESOURCE_ID_25_plus_button);
      s_res_down_arrow_button = gbitmap_create_with_resource(RESOURCE_ID_25_minus_button);
    } else {
      s_res_up_arrow_button = gbitmap_create_with_resource(RESOURCE_ID_5_plus_button);
      s_res_down_arrow_button = gbitmap_create_with_resource(RESOURCE_ID_5_minus_button);
    }
    s_res_check_mark_button = gbitmap_create_with_resource(RESOURCE_ID_check_mark_button);
    
    // s_actionbarlayer_1
    s_actionbarlayer_1 = action_bar_layer_create();
    action_bar_layer_add_to_window(s_actionbarlayer_1, s_weight_window);
    action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_up_arrow_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_check_mark_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_down_arrow_button);
    layer_add_child(window_get_root_layer(s_weight_window), (Layer *)s_actionbarlayer_1);
    
    window_set_click_config_provider(s_weight_window, weight_click_config_provider);
    window_set_window_handlers(s_weight_window, (WindowHandlers) {
      .load = weight_window_load,
      .unload = weight_window_unload
    });
  }
}