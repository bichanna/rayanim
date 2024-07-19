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
  uint32_t elapsed_time;

  bool (*update)(void *, uint32_t);
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
RA_Animation *RA_AnimationList_getAt(RA_AnimationList *anim_list, uint32_t idx);
void RA_AnimationList_set(RA_AnimationList *anim_list, uint32_t idx, RA_Animation *new_anim);
bool RA_AnimationList_contains(RA_AnimationList *anim_list, RA_Animation *anim);
void RA_AnimationList_destroy(RA_AnimationList *anim_list);

void RA_Object_init(RA_Object *obj, Vector2 position, void (*render)(void *));

void RA_Animation_init(RA_Animation *anim,
                       RA_Object *obj,
                       uint32_t duration,
                       bool (*update)(void *, uint32_t),
                       void (*interpolate)(void *, float));
void RA_Animation_defaultInit(RA_Animation *anim,
                              RA_Object *obj,
                              uint32_t duration,
                              void (*interpolate)(void *, float));
bool RA_Animation_defaultUpdate(void *any, uint32_t dt);

void RA_Scene_init(RA_Scene *scene, const char *title, int width, int height, Color color);
void RA_Scene_play(RA_Scene *scene, RA_Animation *anim);
void RA_Scene_render(RA_Scene *scene);
void RA_Scene_update(RA_Scene *scene, uint32_t dt);
void RA_Scene_destroy(RA_Scene *scene);

void playScene(RA_Scene *scene);
void recordScene(RA_Scene *scene);

#endif  // RAYANIM_H
