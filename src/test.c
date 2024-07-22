#include "rayanim.h"

int main(void) {
  RA_Scene scene;
  RA_Scene_init(&scene, "Test scene", 2400, 1600, RAYWHITE);

  // RA_Circle circle1;
  // RA_Circle_defaultInit(&circle1, (Vector2){800, 500}, 200);
  // RA_Animation circle1_anim;
  // RA_CircleAnimation_defaultInit(&circle1_anim, &circle1);

  // RA_Circle circle2;
  // RA_Circle_defaultInit(&circle2, (Vector2){1600, 1000}, 400);
  // RA_Animation circle2_anim;
  // RA_CircleAnimation_defaultInit(&circle2_anim, &circle2);

  // RA_Rectangle rect1;
  // RA_Rectangle_defaultInit(&rect1, (Vector2){100, 1200}, 900, 300);
  // RA_Animation rect1_anim;
  // RA_RectangleAnimation_defaultInit(&rect1_anim, &rect1);

  // RA_Animation wait1sec;
  // RA_WaitAnimation_init(&wait1sec, 2.0f);

  // circle1_anim.duration = 0.5;

  // circle2.outline_color = DARKGREEN;
  // circle2.inner_color = GREEN;
  // circle2.base.render = RA_Circle_fillInnerRender;
  // circle2_anim.duration = 4;

  // rect1.base.render = RA_Rectangle_fillInnerRender;

  RA_Animation wait1sec = RA_WaitAnimation_create(1.0f);

  RA_Circle circle1 = RA_Circle_create((Vector2){800, 500}, 200);
  circle1.inner_color = GRAY;
  circle1.outline_color = DARKGRAY;
  circle1.outline_thickness = 60.0f;
  RA_Animation circle1_anim = RA_CircleAnimation_create(&circle1);
  circle1_anim.duration = 1.0f;

  RA_Circle circle2 = RA_Circle_create((Vector2){1600, 1000}, 310);
  circle2.outline_thickness = 20.0f;
  circle2.base.render = RA_Circle_fillInnerRender;
  RA_Animation circle2_anim = RA_CircleAnimation_create(&circle2);
  circle2_anim.duration = 1.8f;

  RA_Rectangle rect1 = RA_Rectangle_create((Vector2){300, 700}, 2000, 600);
  rect1.base.render = RA_Rectangle_fillInnerRender;
  rect1.outline_thickness = 10.0f;
  RA_Animation rect1_anim = RA_RectangleAnimation_create(&rect1);
  rect1_anim.duration = 2.2f;

  RA_Animation circle1_disappear_anim = RA_DisappearAnimation_create((RA_Object *)&circle1);

  RA_Animation *sync_anims[] = {&circle1_anim, &circle2_anim};
  RA_SyncAnimation circles_anim = RA_SyncAnimation_create(sync_anims, 2);

  RA_Scene_play(&scene, &wait1sec);
  RA_Scene_play(&scene, (RA_Animation *)&circles_anim);
  RA_Scene_play(&scene, &rect1_anim);
  RA_Scene_play(&scene, &wait1sec);
  RA_Scene_play(&scene, &circle1_disappear_anim);

  startScene(&scene);

  RA_Scene_destroy(&scene);

  return 0;
}
