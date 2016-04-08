#include <pebble.h>
#include <stdio.h>
#include "weight.h"
#include "mini-printf.h"

static TextLayer *s_text_layer;
static TextLayer *s_text_layer2;
static TextLayer *s_plus_minus_layer;

static int s_max_weight = 1000;
static int s_current_weight = 45;

char weight_str[4];

static void weight_select_click_handler(ClickRecognizerRef recognize, void *context) {
  
}

static void weight_down_click_handler(ClickRecognizerRef recognize, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "down");
  s_current_weight = s_current_weight - 5;
  if (s_current_weight < 0) {
    s_current_weight = s_max_weight;
  }
  mini_snprintf(weight_str, 20, "%d", s_current_weight);
  text_layer_set_text(s_text_layer2, weight_str);
}

static void weight_up_click_handler(ClickRecognizerRef recognize, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "up");
  s_current_weight = s_current_weight + 5;
  if (s_current_weight > s_max_weight) {
    s_current_weight = 0;
  }
  mini_snprintf(weight_str, 20, "%d", s_current_weight);
  text_layer_set_text(s_text_layer2, weight_str);
  
}

static void weight_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, weight_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, weight_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, weight_up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, weight_up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, weight_down_click_handler);
}

static void rep_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-40, 20}, .size = { 80, 60 } });
  text_layer_set_text(s_text_layer, exercise_string);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  
  mini_snprintf(weight_str, 20, "%d", s_current_weight);
  s_text_layer2 = text_layer_create((GRect) {.origin = {bounds.size.w/2-30, bounds.size.h/2-20}, .size = { 60, 28 } });
  text_layer_set_text(s_text_layer2, weight_str);
  text_layer_set_text_alignment(s_text_layer2, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer2));
  
  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-50, bounds.size.h - 39}, .size = { 100, 60 } });
  text_layer_set_text(s_text_layer, "Select a Weight");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  
  s_plus_minus_layer = text_layer_create((GRect) {.origin = {bounds.size.w - 32, bounds.size.h/2 -45}, .size = { 15, 25 } });
  text_layer_set_text(s_plus_minus_layer, "+5");
  text_layer_set_text_alignment(s_plus_minus_layer, GTextAlignmentCenter);
  text_layer_set_font(s_plus_minus_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_plus_minus_layer));
  
  s_plus_minus_layer = text_layer_create((GRect) {.origin = {bounds.size.w - 32, bounds.size.h/2 -12}, .size = { 20, 25 } });
  text_layer_set_text(s_plus_minus_layer, "OK");
  text_layer_set_text_alignment(s_plus_minus_layer, GTextAlignmentCenter);
  text_layer_set_font(s_plus_minus_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_plus_minus_layer));

  
  s_plus_minus_layer = text_layer_create((GRect) {.origin = {bounds.size.w - 32, bounds.size.h/2 +25}, .size = { 15, 25 } });
  text_layer_set_text(s_plus_minus_layer, "-5");
  text_layer_set_text_alignment(s_plus_minus_layer, GTextAlignmentCenter);
  text_layer_set_font(s_plus_minus_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_plus_minus_layer));
  
}


static void rep_window_unload(Window *window) {
  window_destroy(window);
  s_rep_window = NULL;
  init_rep_window();
}

void init_rep_window(void) {
  if (!s_rep_window) {
    s_rep_window = window_create();
    window_set_click_config_provider(s_rep_window, weight_click_config_provider);
    window_set_window_handlers(s_rep_window, (WindowHandlers) {
      .load = rep_window_load,
      .unload = rep_window_unload
    });
  }
}