#include <pebble.h>
#include <timer.h>

static TextLayer *s_text_layer;
static AppTimer *s_timer;

static void timer_callback(void *data) {
  text_layer_set_text(s_text_layer, "Timer happened!");
  const uint32_t seg[] = { 600, 200, 600, 200, 600 };
  VibePattern pattern = {
    .durations =  seg,
    .num_segments = ARRAY_LENGTH(seg)
  };
  vibes_enqueue_custom_pattern(pattern);
}

static void timer_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_text_layer = text_layer_create(PBL_IF_RECT_ELSE(bounds, grect_inset(bounds, GEdgeInsets(74, 0, 0, 0))));
  text_layer_set_text(s_text_layer, "Waiting for timer...");
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));

  s_timer = app_timer_register(10000, timer_callback, NULL);
}

static void timer_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
  s_timer_window = NULL;
}

void init_timer_window(void) {
  s_timer_window = window_create();
  window_set_window_handlers(s_timer_window, (WindowHandlers) {
    .load = timer_window_load,
    .unload = timer_window_unload,
  });
}

static void deinit(void) {
  window_destroy(s_timer_window);
}