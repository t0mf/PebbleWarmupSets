#include <pebble.h>
#include <timer.h>
#include "ftoa.h"

static TextLayer *s_text_layer;
static TextLayer *s_time_layer;
static AppTimer *s_timer;
static GBitmap *s_res_15_plus_button;
static GBitmap *s_res_play_button;
static GBitmap *s_res_15_minus_button;
static GBitmap *s_res_pause_button;
static GBitmap *s_res_reset_button;
static GBitmap *s_res_play_reset_button;
static ActionBarLayer *s_actionbarlayer_1;
int hours = 0;
int minutes = 3;
int seconds = 30;
char hours_buff[10];
char seconds_buff[10];
char minutes_buff[10];
char timer_buff[10];
bool timer_begin_state = true;
bool timer_edit_state = true;
bool timer_running_state = false;
bool timer_finished_state = false;
const uint32_t seg[] = { 400, 300, 300};

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
      static char s_time_text[] = "00:00   ";
      clock_copy_time_string(s_time_text, sizeof(s_time_text));
      text_layer_set_text(s_time_layer,s_time_text);
  }
  
  if (timer_finished_state == false && timer_running_state == true) { // if timer is running and not finished
    seconds -= units_changed;

    if ( seconds < 0 ) { seconds = 59; minutes -= 1; }
    if ( minutes < 0 && hours != 0) { minutes = 59; hours -= 1; }
    else if ( minutes < 0 && hours <= 0 ) { hours = 0; minutes = 0; seconds = 0; }

    snprintf(timer_buff, sizeof(timer_buff), "%02d:%02d:%02d", hours, minutes, seconds);
    text_layer_set_text(s_text_layer, timer_buff);

    if ( hours <= 0 && minutes <= 0 && seconds <= 0 ) {
      timer_finished_state = true;
      timer_running_state = false;
      text_layer_set_text(s_text_layer, "Timer Up!");
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, NULL);
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_reset_button);
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, NULL);
    }
  } else if (timer_finished_state == true && timer_running_state == false) {
    VibePattern pattern = {
      .durations =  seg,
      .num_segments = ARRAY_LENGTH(seg)
    };
    vibes_enqueue_custom_pattern(pattern);
  }
}

static void timer_select_click_handler(ClickRecognizerRef recognize, void *context) {
  if (timer_begin_state == true) { // If is in begin state, start the timer by
    timer_begin_state = false; // No longer being in the begin state
    timer_edit_state = false; // No longer allow to edit
    timer_running_state = true; // Now in running state
    
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, NULL);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_pause_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, NULL);
    
  } else {
    if (timer_running_state == true) { // If timer is running, pause
      timer_running_state = false;
      timer_edit_state = true;
      
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_15_plus_button);
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_reset_button);
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_15_minus_button);
    } else { // Else if paused, restart start timer
      timer_running_state = true;
      timer_edit_state = false;
      
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, NULL);
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_pause_button);
      action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, NULL);
    }
  }
  if (timer_finished_state == true) { // If timer is finished (alarm going off), reset
    hours = 0; minutes = 3; seconds = 30;
    snprintf(timer_buff, sizeof(timer_buff), "%02d:%02d:%02d", hours, minutes, seconds);
    text_layer_set_text(s_text_layer, timer_buff);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_15_plus_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_reset_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_15_minus_button);
    timer_begin_state = true;
    timer_edit_state = true;
    timer_running_state = false;
    timer_finished_state = false;
  }
}

static void timer_down_click_handler(ClickRecognizerRef recognize, void *context) {
  if (timer_edit_state == true) {
    seconds -= 15;
    check_time();
  }
}

static void timer_up_click_handler(ClickRecognizerRef recognize, void *context) {
  if (timer_edit_state == true) {
    seconds += 15;
    check_time();
  }
}

static void check_time(void) {
  if ( seconds > 59 ) { seconds = 0; minutes += 1; }
  if ( seconds < 0 ) { seconds = 45; minutes -= 1; }
  if ( minutes > 59 ) { minutes = 0; hours += 1; }
  if ( minutes < 0 ) { minutes = 59; hours -= 1; }
  if ( hours > 23 ) { hours = 0; }
  if ( hours < 0 ) { hours = 23; minutes = 59; seconds = 45;}
  
  snprintf(timer_buff, sizeof(timer_buff), "%02d:%02d:%02d", hours, minutes, seconds);
  text_layer_set_text(s_text_layer, timer_buff);
}


static void long_timer_select_click_handler (ClickRecognizerRef recognize, void *context) {
  if (timer_edit_state == true) {
      hours = 0; minutes = 3; seconds = 30;
      snprintf(timer_buff, sizeof(timer_buff), "%02d:%02d:%02d", hours, minutes, seconds);
      text_layer_set_text(s_text_layer, timer_buff);
      timer_begin_state = true;
      timer_edit_state = true;
      timer_running_state = false;
      timer_finished_state = false;
  }
}

static void timer_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, timer_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, timer_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, timer_up_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 500, long_timer_select_click_handler, NULL);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 50, timer_up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 50, timer_down_click_handler);
}

static void timer_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_text_layer = text_layer_create((GRect) {.origin = {bounds.size.w/2-42, bounds.size.h/2-18}, .size = { 85, 30 } });
  

  snprintf(timer_buff, sizeof(timer_buff), "%02d:%02d:%02d", hours, minutes, seconds);
  text_layer_set_text(s_text_layer, timer_buff);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
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

static void timer_window_unload(Window *window) {
  window_stack_remove(s_timer_window,true);
  window_destroy(s_timer_window);
  
  timer_running_state = false;
  timer_edit_state = true;
      
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_15_plus_button);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_reset_button);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_15_minus_button);
  s_timer_window = NULL;
}

void init_timer_window(void) {
  s_timer_window = window_create();
  
  #ifndef PBL_SDK_3
      window_set_fullscreen(s_timer_window, true);
  #endif
    s_res_15_plus_button = gbitmap_create_with_resource(RESOURCE_ID_15_plus_button);
    s_res_play_button = gbitmap_create_with_resource(RESOURCE_ID_play_button);
    s_res_15_minus_button = gbitmap_create_with_resource(RESOURCE_ID_15_minus_button);
    s_res_pause_button = gbitmap_create_with_resource(RESOURCE_ID_pause_button);
    s_res_reset_button = gbitmap_create_with_resource(RESOURCE_ID_reset_button);
    s_res_play_reset_button = gbitmap_create_with_resource(RESOURCE_ID_play_reset_button);
    
    // s_actionbarlayer_1
    s_actionbarlayer_1 = action_bar_layer_create();
    action_bar_layer_add_to_window(s_actionbarlayer_1, s_timer_window);
    action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_15_plus_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_reset_button);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_15_minus_button);
    layer_add_child(window_get_root_layer(s_timer_window), (Layer *)s_actionbarlayer_1);
  
  window_set_click_config_provider(s_timer_window, timer_click_config_provider);
  window_set_window_handlers(s_timer_window, (WindowHandlers) {
    .load = timer_window_load,
    .disappear = timer_window_unload, 
  });
}