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

static int s_max_weight = 500;
char weight_buff[10];

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
  text_layer_set_text(s_text_layer2, weight_buff);
}

static void weight_up_click_handler(ClickRecognizerRef recognize, void *context) {
  m_weight_d = m_weight_d + step_size[unit_system];

  if (m_weight_d > s_max_weight) {
    m_weight_d = 0;
  }
  ftoa(weight_buff, m_weight_d, 2);
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
  
  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-60, 20}, .size = { 120, 60 } });
  text_layer_set_text(s_text_layer, exercise_name_strings[exercise_int]);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  
  if (persist_read_int(((exercise_int + 3)+(unit_system*7))))
  { m_weight_d = persist_read_int(((exercise_int + 3)+(unit_system*7))); } 
  
  int tmp = m_weight_d;
  if (unit_system == 1 && (tmp * 2) % 5 == 4) { m_weight_d += .5; }
  
  ftoa(weight_buff, m_weight_d, 3);
  s_text_layer2 = text_layer_create((GRect) {.origin = {bounds.size.w/2-30, bounds.size.h/2-20}, .size = { 60, 28 } });
  text_layer_set_text_alignment(s_text_layer2, GTextAlignmentCenter);
  text_layer_set_text(s_text_layer2, weight_buff);
  text_layer_set_font(s_text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer2));
  
  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-50, bounds.size.h - 39}, .size = { 100, 60 } });
  text_layer_set_text(s_text_layer, "Select a Weight");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  
  s_plus_minus_layer = text_layer_create((GRect) {.origin = {bounds.size.w - 42, bounds.size.h/2 -45}, .size = { 25, 25 } });
  
  if (unit_system == 0) { text_layer_set_text(s_plus_minus_layer, "+5"); } 
  else if (unit_system == 1) { text_layer_set_text(s_plus_minus_layer, "+2.5"); }
  text_layer_set_text_alignment(s_plus_minus_layer, GTextAlignmentRight);
  text_layer_set_font(s_plus_minus_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_plus_minus_layer));
  
  s_plus_minus_layer = text_layer_create((GRect) {.origin = {bounds.size.w - 28, bounds.size.h/2 -12}, .size = { 20, 25 } });
  text_layer_set_text(s_plus_minus_layer, "OK");
  text_layer_set_text_alignment(s_plus_minus_layer, GTextAlignmentRight);
  text_layer_set_font(s_plus_minus_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_plus_minus_layer));

  s_plus_minus_layer = text_layer_create((GRect) {.origin = {bounds.size.w - 42, bounds.size.h/2 +25}, .size = { 25, 25 } });
  if (unit_system == 0) { text_layer_set_text(s_plus_minus_layer, "-5"); } 
  else if (unit_system == 1) { text_layer_set_text(s_plus_minus_layer, "-2.5"); }
  text_layer_set_text_alignment(s_plus_minus_layer, GTextAlignmentRight);
  text_layer_set_font(s_plus_minus_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_plus_minus_layer));
  
  s_time_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 20));
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
}

void init_weight_window(void) {
  if (!s_weight_window) {
    s_weight_window = window_create();
    window_set_click_config_provider(s_weight_window, weight_click_config_provider);
    window_set_window_handlers(s_weight_window, (WindowHandlers) {
      .load = weight_window_load,
      .unload = weight_window_unload
    });
  }
}