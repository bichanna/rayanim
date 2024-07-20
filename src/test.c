#include "rayanim.h"

int main(void) {
  RA_Scene scene;
  RA_Scene_init(&scene, "Test scene", 2400, 1600, RAYWHITE);

  RA_Circle circle1;
  RA_Circle_defaultInit(&circle1, (Vector2){800, 500}, 200);
  RA_Animation circle1_anim;
  RA_CircleAnimation_defaultInit(&circle1_anim, &circle1);

  RA_Circle circle2;
  RA_Circle_defaultInit(&circle2, (Vector2){1600, 1000}, 400);
  RA_Animation circle2_anim;
  RA_CircleAnimation_defaultInit(&circle2_anim, &circle2);

  RA_Animation wait2secs;
  RA_WaitAnimation_init(&wait2secs, 2.0f);

  circle1_anim.duration = 0.5;

  circle2.outline_color = DARKGREEN;
  circle2.circle_color = GREEN;
  circle2.base.render = RA_Circle_fillInnerRender;
  circle2_anim.duration = 4;

  RA_Scene_play(&scene, &wait2secs);
  RA_Scene_play(&scene, &circle1_anim);
  RA_Scene_play(&scene, &wait2secs);
  RA_Scene_play(&scene, &circle2_anim);

  playScene(&scene);

  RA_Scene_destroy(&scene);

  return 0;
}
