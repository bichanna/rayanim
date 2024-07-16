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

void plug_post_reload(void *state) { plug = state; }

void plug_update(void) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  float t = GetTime();

  float rw = 100.0;
  float rh = 100.0;

  BeginDrawing();

  ClearBackground(GetColor(0x181818FF));
  DrawCircle(w * (sinf(t * 2) + 1.0) * 0.5, h * (cosf(t * 10) + 1.0) * 0.5, 30,
             GREEN);
  DrawRectangle((w - rw) * (sinf(t * 3) + 1.0) * 0.5,
                (h - rh) * (cosf(t * 0.5) + 1.0) * 0.5, rw, rh, RED);

  EndDrawing();
}

void plug_destroy(void) { free(plug); }
