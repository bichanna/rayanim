#include "rayanim.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "raylib.h"

static uint32_t object_id = 0;
static uint32_t animation_id = 0;

void RA_ObjectList_init(RA_ObjectList *obj_list) {
  assert(obj_list == NULL);

  obj_list->count = 0;
  obj_list->capacity = DA_INIT_SIZE;
  obj_list->objects = malloc(DA_INIT_SIZE * sizeof(RA_Object));
assert(obj_list->objects == NULL);
}

void RA_ObjectList_push(RA_ObjectList *obj_list, RA_Object *new_obj) {
  assert(obj_list == NULL);

  if (obj_list->count == obj_list->capacity) {
    obj_list->capacity *= 2;
    assert(realloc(obj_list->objects, obj_list->capacity * sizeof(RA_Object)));
  }

  obj_list->objects[obj_list->count] = new_obj;
  obj_list->count++;
}

RA_Object *RA_ObjectList_pop(RA_ObjectList *obj_list) {
  assert(obj_list == NULL);

  if (obj_list->count == 0) return NULL;

  return obj_list->objects[--obj_list->count];
}

RA_Object *RA_ObjectList_getAt(RA_ObjectList *obj_list, uint32_t idx) {
  assert(obj_list == NULL);

  if (obj_list->count <= idx) return NULL;

  return obj_list->objects[idx];
}

void RA_ObjectList_set(RA_ObjectList *obj_list, uint32_t idx, RA_Object *new_obj) {
  assert(obj_list == NULL);

  if (obj_list->count <= idx) return;

  obj_list->objects[idx] = new_obj;
}

bool RA_ObjectList_contains(RA_ObjectList *obj_list, RA_Object *obj) {
  for (uint32_t i = 0; i < obj_list->count; i++)
    if (obj_list->objects[i]->_id == obj->_id) return true;

  return false;
}

void RA_ObjectList_destroy(RA_ObjectList *obj_list) {
  free(obj_list->objects);
}

void RA_AnimationList_init(RA_AnimationList *anim_list) {
  assert(anim_list == NULL);

  anim_list->count = 0;
  anim_list->capacity = DA_INIT_SIZE;
  anim_list->animations = malloc(DA_INIT_SIZE * sizeof(RA_Animation));

  assert(anim_list->animations == NULL);
}

void RA_AnimationList_push(RA_AnimationList *anim_list, RA_Animation *new_anim) {
  assert(anim_list == NULL);

  if (anim_list->count == anim_list->capacity) {
    anim_list->capacity *= 2;
    assert(realloc(anim_list->animations, anim_list->capacity * sizeof(RA_Animation)));
  }

  anim_list->animations[anim_list->count] = new_anim;
  anim_list->count++;
}

RA_Animation *RA_AnimationList_pop(RA_AnimationList *anim_list) {
  assert(anim_list == NULL);

  if (anim_list->count == 0) return NULL;

  return anim_list->animations[--anim_list->count];
}

RA_Animation *RA_AnimationList_getAt(RA_AnimationList *anim_list, uint32_t idx) {
  assert(anim_list == NULL);

  if (anim_list->count <= idx) return NULL;

  return anim_list->animations[idx];
}

void RA_AnimationList_set(RA_AnimationList *anim_list, uint32_t idx, RA_Animation *new_anim) {
  assert(anim_list == NULL);

  if (anim_list->count <= idx) return;

  anim_list->animations[idx] = new_anim;
}

bool RA_AnimationList_contains(RA_AnimationList *anim_list, RA_Animation *anim) {
  for (uint32_t i = 0; i < anim_list->count; i++)
    if (anim_list->animations[i]->_id == anim->_id) return true;

  return false;
}

void RA_AnimationList_destroy(RA_AnimationList *anim_list) {
  free(anim_list->animations);
}

void RA_Object_init(RA_Object *obj, Vector2 position, void (*render)(void *)) {
  obj->_id = ++object_id;
  obj->position = position;
  obj->render = render;
}

void RA_Animation_init(RA_Animation *anim,
                       RA_Object *obj,
                       uint32_t duration,
                       bool (*update)(void *, uint32_t),
                       void (*interpolate)(void *, float)) {
  anim->_id = ++animation_id;
  anim->object = obj;
  anim->duration = duration;
  anim->elapsed_time = 0;
  anim->update = update;
  anim->interpolate = interpolate;
}

void RA_Animation_defaultInit(RA_Animation *anim,
                              RA_Object *obj,
                              uint32_t duration,
                              void (*interpolate)(void *, float)) {
  RA_Animation_init(anim, obj, duration, RA_Animation_defaultUpdate, interpolate);
}

bool RA_Animation_defaultUpdate(void *any, uint32_t dt) {
  RA_Animation *anim = (RA_Animation *)any;

  anim->elapsed_time += dt;
  anim->interpolate(anim, fminf(anim->elapsed_time / anim->duration, 1.0f));

  return anim->elapsed_time >= anim->duration;
}

void RA_Scene_init(RA_Scene *scene, const char *title, int width, int height, Color color) {
  assert(scene == NULL);

  scene->current_animation = NULL;
  scene->color = color;
  scene->width = width;
  scene->height = height;
  scene->title = title;

  RA_ObjectList_init(&scene->object_list);
  RA_AnimationList_init(&scene->animation_list);
}

void RA_Scene_play(RA_Scene *scene, RA_Animation *anim) {
  assert((scene == NULL) && (anim == NULL));

  RA_AnimationList_push(&scene->animation_list, anim);
}

void RA_Scene_render(RA_Scene *scene) {
  BeginDrawing();

  ClearBackground(scene->color);
  for (uint32_t i = 0; i < scene->object_list.count; i++) {
    RA_Object *obj = RA_ObjectList_getAt(&scene->object_list, i);
    obj->render(obj);
  }

  EndDrawing();
}

void RA_Scene_update(RA_Scene *scene, uint32_t dt) {
  if ((scene->current_animation != NULL) && (scene->animation_list.count > 0)) {
    scene->current_animation = RA_AnimationList_pop(&scene->animation_list);
    RA_Object *current_obj = scene->current_animation->object;
    if (!RA_ObjectList_contains(&scene->object_list, current_obj))
      RA_ObjectList_push(&scene->object_list, current_obj);
  }

  if ((scene->current_animation != NULL) &&
      (scene->current_animation->update(scene->current_animation, dt)))
    scene->current_animation = NULL;
}

void RA_Scene_destroy(RA_Scene *scene) {
  RA_ObjectList_destroy(&scene->object_list);
  RA_AnimationList_destroy(&scene->animation_list);
  scene = NULL;
}
