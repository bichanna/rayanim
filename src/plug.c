#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  Color background_color;
} Plug;

static Plug *plug = NULL;

void plug_init(void) {
  plug = malloc(sizeof(*plug));
  assert(plug != NULL);

  memset(plug, 0, sizeof(*plug));
  plug->background_color = RED;

  TraceLog(LOG_INFO, "---------------------");
  TraceLog(LOG_INFO, "Initialized plugin");
  TraceLog(LOG_INFO, "---------------------");
}

void *plug_pre_reload(void) { return plug; }

void plug_post_reload(void *state) {
  plug = state;
}

void plug_update(void) {
  float width = GetScreenWidth();
  float dt = GetFrameTime();

  BeginDrawing();
  ClearBackground(GetColor(0x181818FF));
  DrawRectangle(width * (sinf(dt) + 1.0) * 0.5, 0, 100, 100, RED);
  EndDrawing();
}

void plug_destroy(void) {
  free(plug);
}
