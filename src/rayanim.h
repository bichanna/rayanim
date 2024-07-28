#ifndef RAYANIM_H
#define RAYANIM_H

#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

#define DA_INIT_SIZE 12

typedef struct Scene Scene;

typedef int FontIndex;
typedef int TextureIndex;

Texture textures[255];
unsigned char textureCount = 0;

Font fonts[255];
unsigned char fontCount = 0;

typedef struct RAObject {
  int _id;
  Vector2 position;
  Color color;

  void (*render)(void *);
} RAObject;

typedef struct RAObjects {
  RAObject **objects;
  int count;
  int capacity;
} RAObjects;

typedef struct Animation {
  int _id;
  RAObject *object;
  float duration;
  float elapsedTime;
  bool done;

  bool (*update)(void *, float);
  void (*interpolate)(void *, float);
  void (*pushToObjects)(Scene *);
} Animation;

typedef struct Animations {
  Animation **animations;
  int count;
  int capacity;
} Animations;

struct Scene {
  RAObjects objects;
  Animations animations;
  Animation *currentAnimation;
  Color color;
  const char *title;
  int width;
  int height;
};

void initRAObjects(RAObjects *objects);
void pushToRAObjects(RAObjects *objects, RAObject *newObj);
RAObject *popFromRAObjects(RAObjects *objects);
RAObject *getFromRAObjects(RAObjects *objects, int idx);
void setToRAObjects(RAObjects *objects, int idx, RAObject *newObj);
bool containsInRAObjects(RAObjects *objects, RAObject *obj);
void destroyRAObjects(RAObjects *objects);

void initAnimations(Animations *anims);
void pushToAnimations(Animations *anims, Animation *newAnim);
Animation *popFromAnimations(Animations *anims);
Animation *popFirstFromAnimations(Animations *anims);
Animation *getFromAnimations(Animations *anims, int idx);
void setToAnimations(Animations *anims, int idx, Animation *newAnim);
bool containsInAnimations(Animations *anims, Animation *anim);
void destroyAnimations(Animations *anims);

void initRAObject(RAObject *obj, Vector2 position, Color color, void (*render)(void *));
void initEmptyRAObject(RAObject *obj);
void renderEmptyRAObject(void *self);

void initAnimation(Animation *anim,
                   RAObject *obj,
                   float duration,
                   bool (*update)(void *, float),
                   void (*interpolate)(void *, float),
                   void (*pushToObjects)(Scene *));
void initDefaultAnimation(Animation *anim,
                          RAObject *obj,
                          float duration,
                          void (*interpolate)(void *, float));
bool updateDefaultAnimation(void *self, float dt);
void pushToObjectsDefaultAnimation(Scene *scene);

void initScene(Scene *scene, const char *title, int width, int height, Color color);
void initDefaultScene(Scene *scene, const char *title);
void playAnimation(Scene *scene, Animation *anim);
void playAnimations(Scene *scene, Animation **anims, int animCount);
void renderScene(Scene *scene);
void updateScene(Scene *scene, float dt);
void destroyScene(Scene *scene);

void startScene(Scene *scene);
void recordScene(Scene *scene);

// ------------------------------ Built-In RAObjects & Animations ------------------------------

// --------------- RACircle ---------------

typedef struct RACircle {
  RAObject base;
  float radius;
  float outlineThickness;
  int segments;
  Color outlineColor;

  float angle;
} RACircle;

void initCircle(RACircle *circle,
                Vector2 center,
                float radius,
                float outlineThickness,
                int segments,
                Color innerColor,
                Color outlineColor,
                void (*render)(void *));
void initDefaultCircle(RACircle *circle, Vector2 center, float radius);
RACircle createCircle(Vector2 center, float radius);
void renderDefaultCircle(void *self);
void renderFillInnerCircle(void *self);
void initCircleAnimation(Animation *anim,
                         RACircle *circle,
                         float duration,
                         bool (*update)(void *, float),
                         void (*interpolate)(void *, float));
void initDefaultCircleAnimation(Animation *anim, RACircle *circle);
Animation createCircleAnimation(RACircle *circle);
void interpolateDefaultCircleAnimation(void *self, float time);

// --------------- RACircle ---------------

// ------------- RARectangle --------------

typedef struct RARectangle {
  RAObject base;
  float width;
  float height;
  float outlineThickness;
  Color outlineColor;

  float firstQuarter;
  float secondQuarter;
  float thirdQuarter;
  float lastQuarter;
} RARectangle;

void initRectangle(RARectangle *rect,
                   Vector2 position,
                   float width,
                   float height,
                   float outlineThickness,
                   Color innerColor,
                   Color outlineColor,
                   void (*render)(void *));
void initDefaultRectangle(RARectangle *rect, Vector2 position, float width, float height);
RARectangle createRectangle(Vector2 position, float width, float height);
void renderDefaultRectangle(void *self);
void renderFillInnerRectangle(void *self);
void initRectangleAnimation(Animation *anim,
                            RARectangle *rect,
                            float duration,
                            bool (*update)(void *, float),
                            void (*interpolate)(void *, float));
void initDefaultRectangleAnimation(Animation *anim, RARectangle *rect);
Animation createRectangleAnimation(RARectangle *rect);
void interpolateDefaultRectangleAnimation(void *self, float time);

// ------------- RARectangle --------------

// -------------- RASquare ----------------

void initSquare(RARectangle *square,
                Vector2 position,
                float length,
                float outlineThickness,
                Color innerColor,
                Color outlineColor,
                void (*render)(void *));
void initDefaultSquare(RARectangle *square, Vector2 position, float length);
RARectangle createSquare(Vector2 position, float length);

// -------------- RASquare ----------------

// ---------------- Delay -----------------

void initDelayAnimation(Animation *anim, float duration);
Animation createDelayAnimation(float duration);
void interpolateDelayAnimation(void *self, float time);

// ---------------- Delay -----------------

// ---------------- FadeOut ---------------

void initFadeOutAnimation(Animation *anim,
                          RAObject *obj,
                          float duration,
                          bool (*update)(void *, float),
                          void (*interpolate)(void *, float));
void initDefaultFadeOutAnimation(Animation *anim, RAObject *obj);
Animation createFadeOutAnimation(RAObject *obj);
void interpolateDefaultFadeOutAnimation(void *self, float time);
void renderDefaultFadeOutAnimation(void *self);

// ---------------- FadeOut ---------------

// ----------------- Sync -----------------

typedef struct SyncAnimation {
  Animation base;
  Animation **animations;
  int animCount;
} SyncAnimation;

void initSyncAnimation(SyncAnimation *anim,
                       Animation **anims,
                       int animCount,
                       void (*pushToObjects)(Scene *));
void initDefaultSyncAnimation(SyncAnimation *anim, Animation **anims, int animCount);
SyncAnimation createSyncAnimation(Animation **anims, int animCount);
bool updateDefaultSyncAnimation(void *self, float dt);
void interpolateDefaultSyncAnimation(void *self, float time);
void pushToObjectsDefaultSyncAnimation(Scene *scene);

// ----------------- Sync -----------------

// ----------------- Move -----------------

typedef struct MoveAnimation {
  Animation base;
  Animation *targetAnim;
  Vector2 initialPosition;
  Vector2 targetPosition;
} MoveAnimation;

void initMoveAnimation(MoveAnimation *anim,
                       Animation *targetAnim,
                       float duration,
                       Vector2 targetPos,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float),
                       void (*pushToObjects)(Scene *));
void initDefaultMoveAnimation(MoveAnimation *anim, Animation *targetAnim, Vector2 targetPos);
MoveAnimation createMoveAnimation(Animation *targetAnim, Vector2 targetPos);
bool updateDefaultMoveAnimation(void *self, float dt);
void pushToObjectsDefaultMoveAnimation(Scene *scene);

// ----------------- Move -----------------

// ---------------- RAText ----------------

typedef struct RAText {
  RAObject base;
  FontIndex fontIdx;
  float spacing;
  float fontSize;
  char *fullText;
  float charRevealTime;
  size_t displayCharCount;
} RAText;

void initText(RAText *text,
              char *fullText,
              FontIndex fontIdx,
              Color tint,
              float charRevealTime,
              float fontSize,
              float spacing,
              Vector2 pos,
              void (*render)(void *));
void initDefaultText(RAText *text, char *fullText, Vector2 pos);
RAText createText(char *fullText, Vector2 pos);
void renderDefaultText(void *self);
void setFontForText(RAText *text, char *filename);
void setFontForTextEx(
    RAText *text, char *filename, int fontSize, int *codepoints, int codepointCount);
void initTextAnimation(Animation *anim,
                       RAText *text,
                       float duration,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float));
void initDefaultTextAnimation(Animation *anim, RAText *text);
Animation createTextAnimation(RAText *text);
void interpolateDefaultTextAnimation(void *self, float time);

// ---------------- RAText ----------------

// --------------- RAImage ----------------

typedef struct RAImage {
  RAObject base;
  TextureIndex textureIdx;
  char *filename;
  float scale;
} RAImage;

void initImage(
    RAImage *image, char *filename, Vector2 pos, float scale, Color tint, void (*render)(void *));
void initDefaultImage(RAImage *image, char *filename, Vector2 pos);
RAImage createImage(char *filename, Vector2 pos);
void renderDefaultImage(void *self);
void initImageAnimation(Animation *anim,
                        RAImage *image,
                        float duration,
                        bool (*update)(void *, float),
                        void (*interpolate)(void *, float));
void initDefaultImageAnimation(Animation *anim, RAImage *image);
Animation createImageAnimation(RAImage *image);
void interpolateDefaultImageAnimation(void *self, float time);

// --------------- RAImage ----------------

#endif  // RAYANIM_H
