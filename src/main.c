#include "plug.h"
#include <dlfcn.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

char *libplug_path = "builddir/libplug.so";
void *libplug = NULL;

bool reload_plugin() {
  if (libplug != NULL) {
    dlclose(libplug);
  }

  libplug = dlopen(libplug_path, RTLD_NOW);
  if (libplug == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    return false;
  }

  plug_init = dlsym(libplug, "plug_init");
  if (plug_init == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    return false;
  }

  plug_pre_reload = dlsym(libplug, "plug_pre_reload");
  if (plug_pre_reload == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    return false;
  }

  plug_post_reload = dlsym(libplug, "plug_post_reload");
  if (plug_post_reload == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    return false;
  }

  plug_update = dlsym(libplug, "plug_update");
  if (plug_update == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    return false;
  }

  plug_destroy = dlsym(libplug, "plug_destroy");
  if (plug_destroy == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    return false;
  }

  return true;
}

int main(void) {
  if (!reload_plugin())
    return 1;

  float factor = 80.0;

  InitWindow(16 * factor, 9 * factor, "Rayanim");
  SetTargetFPS(60);

  plug_init();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R)) {
      void *state = plug_pre_reload();
      reload_plugin();
      plug_post_reload(state);
    } else if (IsKeyPressed(KEY_Q)) {
      break;
    }

    plug_update();
  }

  plug_destroy();

  CloseWindow();

  return 0;
}
