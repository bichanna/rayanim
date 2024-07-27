#include "rayanim.h"

#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

static uint32_t objectId = 0;
static uint32_t animationId = 0;

void initRAObjects(RAObjects *objects) {
  objects->count = 0;
  objects->capacity = DA_INIT_SIZE;
  objects->objects = malloc(DA_INIT_SIZE * sizeof(RAObject *));
  assert(objects->objects != NULL);
}

void pushToRAObjects(RAObjects *objects, RAObject *newObj) {
  assert(objects != NULL);

  if (objects->count == objects->capacity) {
    objects->capacity *= 2;
    assert(realloc(objects->objects, objects->capacity * sizeof(RAObject *)) != NULL);
  }

  objects->objects[objects->count] = newObj;
  objects->count++;
}

RAObject *popFromRAObjects(RAObjects *objects) {
  assert(objects != NULL);

  if (objects->count == 0) return NULL;

  return objects->objects[--objects->count];
}

RAObject *getFromRAObjects(RAObjects *objects, uint32_t idx) {
  assert(objects != NULL);

  if (objects->count <= idx) return NULL;

  return objects->objects[idx];
}

void setToRAObjects(RAObjects *objects, uint32_t idx, RAObject *newObj) {
  assert(objects != NULL);

  if (objects->count <= idx) return;

  objects->objects[idx] = newObj;
}

bool containsInRAObjects(RAObjects *objects, RAObject *obj) {
  for (uint32_t i = 0; i < objects->count; i++)
    if (objects->objects[i]->_id == obj->_id) return true;

  return false;
}

void destroyRAObjects(RAObjects *objects) {
  free(objects->objects);
}

void initAnimations(Animations *anims) {
  anims->count = 0;
  anims->capacity = DA_INIT_SIZE;
  anims->animations = malloc(DA_INIT_SIZE * sizeof(Animation *));
  assert(anims->animations != NULL);
}

void pushToAnimations(Animations *anims, Animation *newAnim) {
  assert(anims != NULL);

  if (anims->count == anims->capacity) {
    anims->capacity *= 2;
    assert(realloc(anims->animations, anims->capacity * sizeof(Animation *)) != NULL);
  }

  anims->animations[anims->count] = newAnim;
  anims->count++;
}

Animation *popFromAnimations(Animations *anims) {
  assert(anims != NULL);

  if (anims->count == 0) return NULL;

  return anims->animations[--anims->count];
}

Animation *popFirstFromAnimations(Animations *anims) {
  assert(anims != NULL);

  if (anims->count == 0) return NULL;

  Animation *firstAnim = anims->animations[0];

  for (uint32_t i = 1; i < anims->count; i++) anims->animations[i - 1] = anims->animations[i];

  anims->count--;

  return firstAnim;
}

Animation *getFromAnimations(Animations *anims, uint32_t idx) {
  assert(anims != NULL);

  if (anims->count <= idx) return NULL;

  return anims->animations[idx];
}

void setToAnimations(Animations *anims, uint32_t idx, Animation *newAnim) {
  assert(anims != NULL);

  if (anims->count <= idx) return;

  anims->animations[idx] = newAnim;
}

bool containsInAnimations(Animations *anims, Animation *anim) {
  for (uint32_t i = 0; i < anims->count; i++)
    if (anims->animations[i]->_id == anim->_id) return true;

  return false;
}

void destroyAnimations(Animations *anims) {
  free(anims->animations);
}

void initRAObject(RAObject *obj, Vector2 position, Color color, void (*render)(void *)) {
  obj->_id = ++objectId;
  obj->position = position;
  obj->render = render;
  obj->color = color;
}

void initEmptyRAObject(RAObject *obj) {
  initRAObject(obj, (Vector2){0, 0}, (Color){0, 0, 0, 0}, renderEmptyRAObject);
}

void renderEmptyRAObject(void *self) {
  (void)self;
}

void initAnimation(Animation *anim,
                   RAObject *obj,
                   float duration,
                   bool (*update)(void *, float),
                   void (*interpolate)(void *, float),
                   void (*pushToObjects)(Scene *)) {
  anim->_id = ++animationId;
  anim->object = obj;
  anim->duration = duration;
  anim->elapsedTime = 0.0f;
  anim->done = false;
  anim->update = update;
  anim->interpolate = interpolate;
  anim->pushToObjects = pushToObjects;
}

void initDefaultAnimation(Animation *anim,
                          RAObject *obj,
                          float duration,
                          void (*interpolate)(void *, float)) {
  initAnimation(
      anim, obj, duration, updateDefaultAnimation, interpolate, pushToObjectsDefaultAnimation);
}

bool updateDefaultAnimation(void *self, float dt) {
  Animation *anim = (Animation *)self;

  if (anim->done) return true;

  anim->elapsedTime += dt;
  anim->interpolate(anim, fminf(anim->elapsedTime / anim->duration, 1.0f));

  bool completed = anim->elapsedTime >= anim->duration;
  if (completed) anim->done = true;

  return completed;
}

void pushToObjectsDefaultAnimation(Scene *scene) {
  RAObject *currentObj = scene->currentAnimation->object;
  TraceLog(LOG_INFO, "RayAnim: Started Animation #%i", scene->currentAnimation->_id);
  pushToRAObjects(&scene->objects, currentObj);
}

void initScene(Scene *scene, const char *title, int width, int height, Color color) {
  scene->currentAnimation = NULL;
  scene->color = color;
  scene->width = width;
  scene->height = height;
  scene->title = title;

  fonts[0] = GetFontDefault();
  fontCount = 1;

  initRAObjects(&scene->objects);
  initAnimations(&scene->animations);

  InitWindow(scene->width, scene->height, scene->title);
}

void initDefaultScene(Scene *scene, const char *title) {
  initScene(scene, title, 2400, 1600, RAYWHITE);
}

void playAnimation(Scene *scene, Animation *anim) {
  assert((scene != NULL) && (anim != NULL));

  pushToAnimations(&scene->animations, anim);
}

void playAnimations(Scene *scene, Animation **anims, int animCount) {
  for (int i = 0; i < animCount; i++) playAnimation(scene, anims[i]);
}

void renderScene(Scene *scene) {
  BeginDrawing();

  ClearBackground(scene->color);
  for (uint32_t i = 0; i < scene->objects.count; i++) {
    RAObject *obj = getFromRAObjects(&scene->objects, i);
    if (obj != NULL) obj->render(obj);
  }

  EndDrawing();
}

void updateScene(Scene *scene, float dt) {
  if ((scene->currentAnimation == NULL) && (scene->animations.count > 0)) {
    scene->currentAnimation = popFirstFromAnimations(&scene->animations);
    scene->currentAnimation->elapsedTime = 0.0f;
    scene->currentAnimation->done = false;
    scene->currentAnimation->pushToObjects(scene);
    // RAObject *currentObj = scene->currentAnimation->object;
    // TraceLog(LOG_INFO, "RayAnim: Started Animation #%i", scene->currentAnimation->_id);
    // // if (!containsInRAObjects(&scene->objects, currentObj))
    // pushToRAObjects(&scene->objects, currentObj);
  }

  if ((scene->currentAnimation != NULL) &&
      (scene->currentAnimation->update(scene->currentAnimation, dt))) {
    TraceLog(LOG_INFO, "RayAnim: Finished Animation #%i", scene->currentAnimation->_id);
    scene->currentAnimation = NULL;
  }
}

void destroyScene(Scene *scene) {
  destroyRAObjects(&scene->objects);
  destroyAnimations(&scene->animations);
  scene = NULL;

  for (int i = 0; i < textureCount; i++) UnloadTexture(textures[i]);

  for (int i = 0; i < fontCount; i++) UnloadFont(fonts[i]);
}

void startScene(Scene *scene) {
  SetTargetFPS(120);

  float lastTime = GetTime();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q)) break;

    float currentTime = GetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;

    updateScene(scene, dt);
    renderScene(scene);
  }

  CloseWindow();
}

void recordScene(Scene *scene) {
  // TODO: will be implemented later
  scene = scene;
}

// ------------------------------ Built-In RAObjects & Animations ------------------------------

// --------------- RACircle ---------------

void initCircle(RACircle *RACircle,
                Vector2 center,
                float radius,
                float outlineThickness,
                int segments,
                Color innerColor,
                Color outlineColor,
                void (*render)(void *)) {
  initRAObject(&RACircle->base, center, innerColor, render);
  RACircle->radius = radius;
  RACircle->angle = 0.0f;
  RACircle->outlineThickness = outlineThickness;
  RACircle->segments = segments;
  RACircle->outlineColor = outlineColor;
}

void initDefaultCircle(RACircle *RACircle, Vector2 center, float radius) {
  initCircle(RACircle, center, radius, 25.0f, 100, BLUE, DARKBLUE, renderDefaultCircle);
}

RACircle createCircle(Vector2 center, float radius) {
  RACircle RACircle;
  initDefaultCircle(&RACircle, center, radius);
  return RACircle;
}

void renderDefaultCircle(void *self) {
  RACircle *circle = (RACircle *)self;

  float innerRadius = circle->radius - circle->outlineThickness / 2;
  float outerRadius = circle->radius + circle->outlineThickness / 2;
  DrawRing(circle->base.position,
           innerRadius,
           outerRadius,
           0.0f,
           circle->angle,
           circle->segments,
           circle->outlineColor);
}

void renderFillInnerCircle(void *self) {
  RACircle *circle = (RACircle *)self;

  float halfThickness = circle->outlineThickness / 2;

  DrawCircleSector(circle->base.position,
                   circle->radius + halfThickness,
                   0.0f,
                   circle->angle,
                   circle->segments,
                   circle->outlineColor);

  DrawCircleSector(circle->base.position,
                   circle->radius - halfThickness,
                   0.0f,
                   circle->angle,
                   circle->segments,
                   circle->base.color);
}

void initCircleAnimation(Animation *anim,
                         RACircle *RACircle,
                         float duration,
                         bool (*update)(void *, float),
                         void (*interpolate)(void *, float)) {
  initAnimation(
      anim, (RAObject *)RACircle, duration, update, interpolate, pushToObjectsDefaultAnimation);
}

void initDefaultCircleAnimation(Animation *anim, RACircle *RACircle) {
  initCircleAnimation(
      anim, RACircle, 0.8f, updateDefaultAnimation, interpolateDefaultCircleAnimation);
}

Animation createCircleAnimation(RACircle *RACircle) {
  Animation anim;
  initDefaultCircleAnimation(&anim, RACircle);
  return anim;
}
void interpolateDefaultCircleAnimation(void *self, float time) {
  Animation *anim = (Animation *)self;
  RACircle *circle = (RACircle *)anim->object;
  circle->angle = time * 360.0f;
}

// --------------- RACircle ---------------

// ------------- RARectangle --------------

void initRectangle(RARectangle *rect,
                   Vector2 position,
                   float width,
                   float height,
                   float outlineThickness,
                   Color innerColor,
                   Color outlineColor,
                   void (*render)(void *)) {
  initRAObject(&rect->base, position, innerColor, render);
  rect->width = width;
  rect->height = height;
  rect->outlineThickness = outlineThickness;
  rect->outlineColor = outlineColor;

  rect->firstQuarter = 0.0f;
  rect->secondQuarter = 0.0f;
  rect->thirdQuarter = 0.0f;
  rect->lastQuarter = 0.0f;
}

void initDefaultRectangle(RARectangle *rect, Vector2 position, float width, float height) {
  initRectangle(rect, position, width, height, 25.0f, GREEN, DARKGREEN, renderDefaultRectangle);
}

RARectangle createRectangle(Vector2 position, float width, float height) {
  RARectangle rect;
  initDefaultRectangle(&rect, position, width, height);
  return rect;
}

void renderDefaultRectangle(void *self) {
  RARectangle *rect = (RARectangle *)self;

  float x = rect->base.position.x;
  float y = rect->base.position.y;
  float width = rect->width;
  float height = rect->height;
  float halfThickness = rect->outlineThickness / 2.0f;
  float thickness = rect->outlineThickness;
  float firstQuarter = rect->firstQuarter;
  float secondQuarter = rect->secondQuarter;
  float thirdQuarter = rect->thirdQuarter;
  float lastQuarter = rect->lastQuarter;

  DrawLineEx((Vector2){x, y + halfThickness},
             (Vector2){x + (width - thickness) * firstQuarter, y + halfThickness},
             thickness,
             rect->outlineColor);

  DrawLineEx((Vector2){x + width - halfThickness, y},
             (Vector2){x + width - halfThickness, y + (height - thickness) * secondQuarter},
             thickness,
             rect->outlineColor);

  DrawLineEx(
      (Vector2){x + width, y + height - halfThickness},
      (Vector2){(x + width) - (width - thickness) * thirdQuarter, y + height - halfThickness},
      thickness,
      rect->outlineColor);

  DrawLineEx((Vector2){x + halfThickness, y + height},
             (Vector2){x + halfThickness, (y + height) - (height - thickness) * lastQuarter},
             thickness,
             rect->outlineColor);
}

void renderFillInnerRectangle(void *self) {
  RARectangle *rect = (RARectangle *)self;

  float x = rect->base.position.x;
  float y = rect->base.position.y;
  float width = rect->width;
  float height = rect->height;
  float halfThickness = rect->outlineThickness / 2.0f;
  float thickness = rect->outlineThickness;
  float firstQuarter = rect->firstQuarter;
  float secondQuarter = rect->secondQuarter;
  float thirdQuarter = rect->thirdQuarter;
  float lastQuarter = rect->lastQuarter;

  DrawLineEx((Vector2){x, y + halfThickness},
             (Vector2){x + (width - thickness) * firstQuarter, y + halfThickness},
             thickness,
             rect->outlineColor);

  DrawLineEx((Vector2){x + width - halfThickness, y},
             (Vector2){x + width - halfThickness, y + (height - thickness) * secondQuarter},
             thickness,
             rect->outlineColor);

  DrawTriangle((Vector2){x + thickness, y + thickness},
               (Vector2){x + width - thickness, y + (height - thickness) * secondQuarter},
               (Vector2){x + width - thickness, y + thickness},
               rect->base.color);

  DrawLineEx(
      (Vector2){x + width, y + height - halfThickness},
      (Vector2){(x + width) - (width - thickness) * thirdQuarter, y + height - halfThickness},
      thickness,
      rect->outlineColor);

  DrawTriangle((Vector2){x + thickness, y + thickness},
               (Vector2){(x + width) - (width - thickness) * thirdQuarter, y + height - thickness},
               (Vector2){x + width - thickness, y + height - thickness},
               rect->base.color);

  DrawLineEx((Vector2){x + halfThickness, y + height},
             (Vector2){x + halfThickness, (y + height) - (height - thickness) * lastQuarter},
             thickness,
             rect->outlineColor);
}

void initRectangleAnimation(Animation *anim,
                            RARectangle *rect,
                            float duration,
                            bool (*update)(void *, float),
                            void (*interpolate)(void *, float)) {
  initAnimation(
      anim, (RAObject *)rect, duration, update, interpolate, pushToObjectsDefaultAnimation);
}

void initDefaultRectangleAnimation(Animation *anim, RARectangle *rect) {
  initRectangleAnimation(
      anim, rect, 0.7, updateDefaultAnimation, interpolateDefaultRectangleAnimation);
}

Animation createRectangleAnimation(RARectangle *rect) {
  Animation anim;
  initDefaultRectangleAnimation(&anim, rect);
  return anim;
}

void interpolateDefaultRectangleAnimation(void *self, float time) {
  (void)time;

  Animation *anim = (Animation *)self;
  RARectangle *rect = (RARectangle *)anim->object;

  float elapsedTime = anim->elapsedTime;
  float quarterDuration = anim->duration / 4.0f;

  rect->firstQuarter = fminf(elapsedTime / quarterDuration, 1.0f);

  if ((elapsedTime - quarterDuration) > 0)
    rect->secondQuarter = fminf((elapsedTime - quarterDuration) / quarterDuration, 1.0f);

  if ((elapsedTime - quarterDuration * 2) > 0)
    rect->thirdQuarter = fminf((elapsedTime - quarterDuration * 2) / quarterDuration, 1.0f);

  if ((elapsedTime - quarterDuration * 3) > 0)
    rect->lastQuarter = fminf((elapsedTime - quarterDuration * 3) / quarterDuration, 1.0f);
}

// ------------- RARectangle --------------

// -------------- RASquare ----------------

void initSquare(RARectangle *square,
                Vector2 position,
                float length,
                float outlineThickness,
                Color innerColor,
                Color outlineColor,
                void (*render)(void *)) {
  initRectangle(
      square, position, length, length, outlineThickness, innerColor, outlineColor, render);
}

void initDefaultSquare(RARectangle *square, Vector2 position, float length) {
  initDefaultRectangle(square, position, length, length);
}

RARectangle createSquare(Vector2 position, float length) {
  RARectangle square;
  initDefaultSquare(&square, position, length);
  return square;
}

// -------------- RASquare ----------------

// ---------------- RADelay ----------------

static RAObject wait_object;

void initDelayAnimation(Animation *anim, float duration) {
  initEmptyRAObject(&wait_object);
  initAnimation(anim,
                &wait_object,
                duration,
                updateDefaultAnimation,
                interpolateDelayAnimation,
                pushToObjectsDefaultAnimation);
}

Animation createDelayAnimation(float duration) {
  Animation anim;
  initDelayAnimation(&anim, duration);
  return anim;
}

void interpolateDelayAnimation(void *self, float time) {
  (void)time;
  (void)self;
}

// ---------------- RADelay ----------------

// -------------- RAFadeOut -------------

void initFadeOutAnimation(Animation *anim,
                          RAObject *obj,
                          float duration,
                          bool (*update)(void *, float),
                          void (*interpolate)(void *, float)) {
  initAnimation(anim, obj, duration, update, interpolate, pushToObjectsDefaultAnimation);
}

void initDefaultFadeOutAnimation(Animation *anim, RAObject *obj) {
  initFadeOutAnimation(anim, obj, 0.0f, updateDefaultAnimation, interpolateDefaultFadeOutAnimation);
}

Animation createFadeOutAnimation(RAObject *obj) {
  Animation anim;
  initDefaultFadeOutAnimation(&anim, obj);
  return anim;
}

void interpolateDefaultFadeOutAnimation(void *self, float time) {
  (void)time;
  Animation *anim = (Animation *)self;
  anim->object->render = renderDefaultFadeOutAnimation;
}

void renderDefaultFadeOutAnimation(void *self) {
  (void)self;
}

// -------------- RAFadeOut -------------

// ---------------- Sync ----------------

void initSyncAnimation(SyncAnimation *anim,
                       Animation **anims,
                       uint8_t animCount,
                       void (*pushToObjects)(Scene *)) {
  initAnimation((Animation *)anim,
                NULL,
                0,
                updateDefaultSyncAnimation,
                interpolateDefaultSyncAnimation,
                pushToObjects);
  anim->animations = anims;
  anim->animCount = animCount;
}

void initDefaultSyncAnimation(SyncAnimation *anim, Animation **anims, uint8_t animCount) {
  initSyncAnimation(anim, anims, animCount, pushToObjectsDefaultSyncAnimation);
}

SyncAnimation createSyncAnimation(Animation **anims, uint8_t animCount) {
  SyncAnimation anim;
  initDefaultSyncAnimation(&anim, anims, animCount);
  return anim;
}

bool updateDefaultSyncAnimation(void *self, float dt) {
  SyncAnimation *anim = (SyncAnimation *)self;

  if (anim->base.done) return true;

  uint8_t completedNum = 0;

  for (uint8_t i = 0; i < anim->animCount; i++) {
    Animation *each = anim->animations[i];

    if (!each->done) {
      each->elapsedTime += dt;
      each->interpolate(each, fminf(each->elapsedTime / each->duration, 1.0f));
    } else {
      completedNum++;
      continue;
    }

    bool completed = (each->elapsedTime >= each->duration);
    if (completed) {
      completedNum++;
      each->done = true;
    }
  }

  return completedNum == anim->animCount;
}

void interpolateDefaultSyncAnimation(void *self, float time) {
  (void)self;
  (void)time;
}

void pushToObjectsDefaultSyncAnimation(Scene *scene) {
  SyncAnimation *anim = (SyncAnimation *)scene->currentAnimation;
  for (uint8_t i = 0; i < anim->animCount; i++) {
    Animation *eachAnim = anim->animations[i];
    RAObject *eachObj = eachAnim->object;
    TraceLog(LOG_INFO, "RayAnim: Started Animation #%i", eachAnim->_id);
    pushToRAObjects(&scene->objects, eachObj);
  }
}

// ---------------- Sync ----------------

// ---------------- Move ----------------

void initMoveAnimation(MoveAnimation *anim,
                       Animation *targetAnim,
                       float duration,
                       Vector2 targetPos,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float),
                       void (*pushToObjects)(Scene *)) {
  assert((targetAnim != NULL) && (targetAnim->object != NULL));
  initAnimation((Animation *)anim, NULL, duration, update, interpolate, pushToObjects);
  anim->targetAnim = targetAnim;
  anim->initialPosition = targetAnim->object->position;
  anim->targetPosition = targetPos;
}

void initDefaultMoveAnimation(MoveAnimation *anim, Animation *targetAnim, Vector2 targetPos) {
  initMoveAnimation(anim,
                    targetAnim,
                    0.8f,
                    targetPos,
                    updateDefaultMoveAnimation,
                    NULL,
                    pushToObjectsDefaultMoveAnimation);
}

MoveAnimation createMoveAnimation(Animation *targetAnim, Vector2 targetPos) {
  MoveAnimation anim;
  initDefaultMoveAnimation(&anim, targetAnim, targetPos);
  return anim;
}

bool updateDefaultMoveAnimation(void *self, float dt) {
  MoveAnimation *anim = (MoveAnimation *)self;
  Animation *targetAnim = anim->targetAnim;

  if (anim->base.done && targetAnim->done) return true;

  float t = fminf(anim->base.elapsedTime / anim->base.duration, 1.0f);
  float dx = (anim->targetPosition.x - anim->initialPosition.x) * t;
  float dy = (anim->targetPosition.y - anim->initialPosition.y) * t;

  anim->targetAnim->object->position.x = anim->initialPosition.x + dx;
  anim->targetAnim->object->position.y = anim->initialPosition.y + dy;

  bool innerAnimCompleted = targetAnim->update(targetAnim, dt);

  anim->base.elapsedTime += dt;

  bool selfCompleted = anim->base.elapsedTime >= anim->base.duration;
  if (selfCompleted) anim->base.done = true;

  return innerAnimCompleted && selfCompleted;
}

void pushToObjectsDefaultMoveAnimation(Scene *scene) {
  MoveAnimation *anim = (MoveAnimation *)scene->currentAnimation;
  RAObject *currentObj = anim->targetAnim->object;
  TraceLog(LOG_INFO, "RayAnim: Started Animation #%i", anim->base._id);
  pushToRAObjects(&scene->objects, currentObj);
}

// ---------------- Move ----------------

// ---------------- RAText ----------------

void initText(RAText *text,
              char *fullText,
              FontIndex fontIdx,
              Color tint,
              float charRevealTime,
              float fontSize,
              float spacing,
              Vector2 pos,
              void (*render)(void *)) {
  initRAObject(&text->base, pos, tint, render);
  text->spacing = spacing;
  text->fontSize = fontSize;
  text->fullText = fullText;
  text->displayCharCount = 0;
  text->charRevealTime = charRevealTime;
  text->fontIdx = fontIdx;
}

void initDefaultText(RAText *text, char *fullText, Vector2 pos) {
  initText(text, fullText, 0, BLACK, 0.03f, 100, 12.5f, pos, renderDefaultText);
}

RAText createText(char *fullText, Vector2 pos) {
  RAText text;
  initDefaultText(&text, fullText, pos);
  return text;
}

void renderDefaultText(void *self) {
  RAText *text = (RAText *)self;

  Font font = fonts[text->fontIdx];
  Color tint = text->base.color;
  char displayText[text->displayCharCount];

  strncpy(displayText, text->fullText, text->displayCharCount - 1);
  displayText[text->displayCharCount - 1] = '\0';

  DrawTextEx(font, displayText, text->base.position, text->fontSize, text->spacing, tint);
}

void setFontForText(RAText *text, char *filename) {
  fonts[fontCount++] = LoadFont(filename);
  text->fontIdx = fontCount - 1;
}

void setFontForTextEx(
    RAText *text, char *filename, int fontSize, int *codepoints, int codepointCount) {
  fonts[fontCount++] = LoadFontEx(filename, fontSize, codepoints, codepointCount);
  text->fontIdx = fontCount - 1;
}

void initTextAnimation(Animation *anim,
                       RAText *text,
                       float duration,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float)) {
  initAnimation(
      anim, (RAObject *)text, duration, update, interpolate, pushToObjectsDefaultAnimation);
}

void initDefaultTextAnimation(Animation *anim, RAText *text) {
  initTextAnimation(anim,
                    text,
                    text->charRevealTime * strlen(text->fullText),
                    updateDefaultAnimation,
                    interpolateDefaultTextAnimation);
}

Animation createTextAnimation(RAText *text) {
  Animation anim;
  initDefaultTextAnimation(&anim, text);
  return anim;
}

void interpolateDefaultTextAnimation(void *self, float time) {
  (void)time;
  Animation *anim = (Animation *)self;
  RAText *text = (RAText *)anim->object;

  if ((anim->elapsedTime >= (text->charRevealTime * text->displayCharCount)) &&
      (text->displayCharCount <= strlen(text->fullText)))
    text->displayCharCount++;
}

// ---------------- RAText ----------------

// --------------- RAImage ----------------

void initImage(
    RAImage *image, char *filename, Vector2 pos, float scale, Color tint, void (*render)(void *)) {
  initRAObject(&image->base, pos, tint, render);
  image->filename = filename;
  image->scale = scale;

  textures[textureCount++] = LoadTexture(filename);
  image->textureIdx = textureCount - 1;
}

void initDefaultImage(RAImage *Texture, char *filename, Vector2 pos) {
  initImage(Texture, filename, pos, 1.0f, RAYWHITE, renderDefaultImage);
}

RAImage createImage(char *filename, Vector2 pos) {
  RAImage image;
  initDefaultImage(&image, filename, pos);
  return image;
}

void renderDefaultImage(void *self) {
  RAImage *image = (RAImage *)self;
  Texture texture = textures[image->textureIdx];
  Color tint = image->base.color;

  DrawTextureEx(texture, image->base.position, 0.0f, image->scale, tint);
}

void initImageAnimation(Animation *anim,
                        RAImage *Texture,
                        float duration,
                        bool (*update)(void *, float),
                        void (*interpolate)(void *, float)) {
  initAnimation(
      anim, (RAObject *)Texture, duration, update, interpolate, pushToObjectsDefaultAnimation);
}

void initDefaultImageAnimation(Animation *anim, RAImage *Texture) {
  initImageAnimation(anim, Texture, 0.8f, updateDefaultAnimation, interpolateDefaultImageAnimation);
}

Animation createImageAnimation(RAImage *Texture) {
  Animation anim;
  initDefaultImageAnimation(&anim, Texture);
  return anim;
}

void interpolateDefaultImageAnimation(void *self, float time) {
  Animation *anim = (Animation *)self;
  RAImage *image = (RAImage *)anim->object;
  image->base.color.a = (uint8_t)(UINT8_MAX * time);
}

// --------------- RAImage ----------------
