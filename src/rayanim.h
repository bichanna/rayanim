#ifndef RAYANIM_H
#define RAYANIM_H

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define DA_INIT_SIZE 12

typedef struct RA_Object {
  uint32_t _id;
  Vector2 position;

  void (*render)(void *);
} RA_Object;

typedef struct RA_ObjectList {
  RA_Object **objects;
  uint32_t count;
  uint32_t capacity;
} RA_ObjectList;

typedef struct RA_Animation {
  uint32_t _id;
  RA_Object *object;
  float duration;
  float elapsed_time;

  bool (*update)(void *, float);
  void (*interpolate)(void *, float);
} RA_Animation;

typedef struct RA_AnimationList {
  RA_Animation **animations;
  uint32_t count;
  uint32_t capacity;
} RA_AnimationList;

typedef struct RA_Scene {
  RA_ObjectList object_list;
  RA_AnimationList animation_list;
  RA_Animation *current_animation;
  Color color;
  const char *title;
  int width;
  int height;
} RA_Scene;

void RA_ObjectList_init(RA_ObjectList *obj_list);
void RA_ObjectList_push(RA_ObjectList *obj_list, RA_Object *new_obj);
RA_Object *RA_ObjectList_pop(RA_ObjectList *obj_list);
RA_Object *RA_ObjectList_getAt(RA_ObjectList *obj_list, uint32_t idx);
void RA_ObjectList_set(RA_ObjectList *obj_list, uint32_t idx, RA_Object *new_obj);
bool RA_ObjectList_contains(RA_ObjectList *obj_list, RA_Object *obj);
void RA_ObjectList_destroy(RA_ObjectList *obj_list);

void RA_AnimationList_init(RA_AnimationList *anim_list);
void RA_AnimationList_push(RA_AnimationList *anim_list, RA_Animation *new_anim);
RA_Animation *RA_AnimationList_pop(RA_AnimationList *anim_list);
RA_Animation *RA_AnimationList_popFirst(RA_AnimationList *anim_list);
RA_Animation *RA_AnimationList_getAt(RA_AnimationList *anim_list, uint32_t idx);
void RA_AnimationList_set(RA_AnimationList *anim_list, uint32_t idx, RA_Animation *new_anim);
bool RA_AnimationList_contains(RA_AnimationList *anim_list, RA_Animation *anim);
void RA_AnimationList_destroy(RA_AnimationList *anim_list);

void RA_Object_init(RA_Object *obj, Vector2 position, void (*render)(void *));
void RA_Object_initEmpty(RA_Object *obj);
void RA_Object_emptyRender(void *self);

void RA_Animation_init(RA_Animation *anim,
                       RA_Object *obj,
                       float duration,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float));
void RA_Animation_defaultInit(RA_Animation *anim,
                              RA_Object *obj,
                              float duration,
                              void (*interpolate)(void *, float));
bool RA_Animation_defaultUpdate(void *self, float dt);

void RA_Scene_init(RA_Scene *scene, const char *title, int width, int height, Color color);
void RA_Scene_play(RA_Scene *scene, RA_Animation *anim);
void RA_Scene_render(RA_Scene *scene);
void RA_Scene_update(RA_Scene *scene, float dt);
void RA_Scene_destroy(RA_Scene *scene);

void playScene(RA_Scene *scene);
void recordScene(RA_Scene *scene);

// ------------------------------ Built-In RA_Objects & RA_Animations ------------------------------

// --------------- RA_Circle ---------------

typedef struct RA_Circle {
  RA_Object base;
  float radius;
  float _angle;
  float outline_thickness;
  int segments;
  Color inner_color;
  Color outline_color;
} RA_Circle;

void RA_Circle_init(RA_Circle *circle,
                    Vector2 center,
                    float radius,
                    float outline_thickness,
                    int segments,
                    Color inner_color,
                    Color outline_color,
                    void (*render)(void *));
void RA_Circle_defaultInit(RA_Circle *circle, Vector2 center, float radius);
RA_Circle RA_Circle_create(Vector2 center, float radius);
void RA_Circle_defaultRender(void *self);
void RA_Circle_fillInnerRender(void *self);
void RA_CircleAnimation_init(RA_Animation *anim,
                             RA_Circle *circle,
                             float duration,
                             bool (*update)(void *, float),
                             void (*interpolate)(void *, float));
void RA_CircleAnimation_defaultInit(RA_Animation *anim, RA_Circle *circle);
RA_Animation RA_CircleAnimation_create(RA_Circle *circle);
void RA_CircleAnimation_defaultInterpolate(void *self, float time);

// --------------- RA_Circle ---------------

// ------------- RA_Rectangle --------------

typedef struct RA_Rectangle {
  RA_Object base;
  float width;
  float height;
  float outline_thickness;
  float _time;
  float _duration;
  float _elapsed_time;
  Color inner_color;
  Color outline_color;
} RA_Rectangle;

void RA_Rectangle_init(RA_Rectangle *rect,
                       Vector2 position,
                       float width,
                       float height,
                       float outline_thickness,
                       Color inner_color,
                       Color outline_color,
                       void (*render)(void *));
void RA_Rectangle_defaultInit(RA_Rectangle *rect, Vector2 position, float width, float height);
RA_Rectangle RA_Rectangle_create(Vector2 position, float width, float height);
void RA_Rectangle_defaultRender(void *self);
void RA_Rectangle_defaultDisappearRender(void *self);
void RA_Rectangle_fillInnerRender(void *self);
void RA_RectangleAnimation_init(RA_Animation *anim,
                                RA_Rectangle *rect,
                                float duration,
                                bool (*update)(void *, float),
                                void (*interpolate)(void *, float));
void RA_RectangleAnimation_defaultInit(RA_Animation *anim, RA_Rectangle *rect);
RA_Animation RA_RectangleAnimation_create(RA_Rectangle *rect);
void RA_RectangleAnimation_defaultInterpolate(void *self, float time);

// ------------- RA_Rectangle --------------

// ---------------- RA_Wait ----------------

void RA_WaitAnimation_init(RA_Animation *anim, float duration);
RA_Animation RA_WaitAnimation_create(float duration);
void RA_WaitAnimation_interpolate(void *self, float time);

// ---------------- RA_Wait ----------------

#endif  // RAYANIM_H
