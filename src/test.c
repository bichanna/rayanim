#include "rayanim.h"
#include "raylib.h"

int main(void) {
  RA_Scene scene;
  RA_Scene_init(&scene, "Test scene", 2400, 1600, RAYWHITE);

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
  rect1_anim.duration = 2.2f; RA_Animation circle1_disappear_anim =
  RA_DisappearAnimation_create((RA_Object *)&circle1);

  RA_Animation *sync_anims[] = {&circle1_anim, &circle2_anim};
  RA_SyncAnimation circles_anim = RA_SyncAnimation_create(sync_anims, 2);

  RA_MoveAnimation rect1_move = RA_MoveAnimation_create(&rect1_anim, (Vector2){200, 600});

  RA_Text text1 = RA_Text_create("Hello, this is rayanim!", (Vector2){800, 800});
  RA_Animation text1_anim = RA_TextAnimation_create(&text1);

  RA_Image image1 = RA_Image_create("/home/nobu/Downloads/bach-fun.png", (Vector2){100, 100});
  RA_Animation image1_anim = RA_ImageAnimation_create(&image1);

  RA_Animation *sync_anims2[] = {&image1_anim, &text1_anim};
  RA_SyncAnimation sync_image1_text1 = RA_SyncAnimation_create(sync_anims2, 2);

  RA_Scene_play(&scene, &wait1sec);
  RA_Scene_play(&scene, (RA_Animation *)&circles_anim);
  RA_Scene_play(&scene, &rect1_anim);
  RA_Scene_play(&scene, (RA_Animation *)&rect1_move);
  RA_Scene_play(&scene, &wait1sec);
  RA_Scene_play(&scene, &circle1_disappear_anim);
  RA_Scene_play(&scene, &wait1sec);
  RA_Scene_play(&scene, (RA_Animation *)&sync_image1_text1);

  startScene(&scene);

  RA_Scene_destroy(&scene);

  return 0;
}
