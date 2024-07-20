#include "rayanim.h"
#include "raylib.h"

int main(void) {
  RA_Scene scene;
  RA_Scene_init(&scene, "Test scene", 2400, 1600, RAYWHITE);

  RA_Circle circle1;
  RA_Circle_defaultInit(&circle1, (Vector2){1200, 800}, 500);
  RA_Animation circle1_anim;
  RA_CircleAnimation_defaultInit(&circle1_anim, &circle1);

  RA_Animation wait;
  RA_WaitAnimation_init(&wait, 3);

  RA_Scene_play(&scene, &wait);
  RA_Scene_play(&scene, &circle1_anim);

  playScene(&scene);

  RA_Scene_destroy(&scene);

  return 0;
}
