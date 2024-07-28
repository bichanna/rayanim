#include "rayanim.h"
#include "raylib.h"

int main(void) {
  Scene scene;
  initScene(&scene, "Test scene", 2400, 1600, RAYWHITE);

  Animation delay1sec = createDelayAnimation(1.0f);

  RACircle circle1 = createCircle((Vector2){800, 500}, 200);
  circle1.base.color = GRAY;
  circle1.outlineColor = DARKGRAY;
  circle1.outlineThickness = 60.0f;
  Animation circle1Anim = createCircleAnimation(&circle1);
  circle1Anim.duration = 1.0f;

  RACircle circle2 = createCircle((Vector2){1600, 1000}, 310);
  circle2.outlineThickness = 20.0f;
  circle2.base.render = renderFillInnerCircle;
  Animation circle2Anim = createCircleAnimation(&circle2);
  circle2Anim.duration = 1.8f;

  RARectangle rect1 = createRectangle((Vector2){300, 700}, 2000, 600);
  rect1.base.render = renderFillInnerRectangle;
  rect1.outlineThickness = 18.0f;
  Animation rect1Anim = createRectangleAnimation(&rect1);
  rect1Anim.duration = 2.2f;
  Animation circle1FadeOut = createFadeOutAnimation((RAObject *)&circle1);

  Animation *syncAnims[] = {&circle1Anim, &circle2Anim};
  SyncAnimation circlesAnim = createSyncAnimation(syncAnims, 2);

  MoveAnimation rect1Move = createMoveAnimation(&rect1Anim, (Vector2){200, 600});

  RAText text1 = createText("Hello, this is rayanim!", (Vector2){800, 800});
  setFontForTextEx(&text1, "Iosevka-Bold.ttf", 255, NULL, 0);
  Animation text1Anim = createTextAnimation(&text1);

  RAImage image1 = createImage("/home/nobu/Downloads/bach-fun.png", (Vector2){100, 100});
  Animation image1Anim = createImageAnimation(&image1);
  Animation image1FadeOut = createFadeOutAnimation((RAObject *)&image1);

  Animation *syncAnims2[] = {&image1Anim, &text1Anim};
  SyncAnimation syncImage1Text1 = createSyncAnimation(syncAnims2, 2);

  RARectangle square1 = createSquare((Vector2){1000, 230}, 350);
  square1.outlineThickness = 0.0f;
  square1.outlineColor = RED;
  square1.base.color = RED;
  square1.base.color.a = 80;
  square1.base.render = renderFillInnerRectangle;
  Animation square1Anim = createRectangleAnimation(&square1);
  square1Anim.duration = 2.5f;

  Animation *anims[] = {&delay1sec,
                        (Animation *)&circlesAnim,
                        &rect1Anim,
                        (Animation *)&rect1Move,
                        &delay1sec,
                        &circle1FadeOut,
                        &delay1sec,
                        (Animation *)&syncImage1Text1,
                        &square1Anim,
                        &image1FadeOut};

  playAnimations(&scene, anims, 10);

  startScene(&scene);

  destroyScene(&scene);

  return 0;
}
