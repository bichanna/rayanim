#include "rayanim.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "raylib.h"

static uint32_t object_id = 0;
static uint32_t animation_id = 0;

void RA_ObjectList_init(RA_ObjectList *obj_list) {
  obj_list->count = 0;
  obj_list->capacity = DA_INIT_SIZE;
  obj_list->objects = malloc(DA_INIT_SIZE * sizeof(RA_Object));
  assert(obj_list->objects != NULL);
}

void RA_ObjectList_push(RA_ObjectList *obj_list, RA_Object *new_obj) {
  assert(obj_list != NULL);

  if (obj_list->count == obj_list->capacity) {
    obj_list->capacity *= 2;
    assert(realloc(obj_list->objects, obj_list->capacity * sizeof(RA_Object)) != NULL);
  }

  obj_list->objects[obj_list->count] = new_obj;
  obj_list->count++;
}

RA_Object *RA_ObjectList_pop(RA_ObjectList *obj_list) {
  assert(obj_list != NULL);

  if (obj_list->count == 0) return NULL;

  return obj_list->objects[--obj_list->count];
}

RA_Object *RA_ObjectList_getAt(RA_ObjectList *obj_list, uint32_t idx) {
  assert(obj_list != NULL);

  if (obj_list->count <= idx) return NULL;

  return obj_list->objects[idx];
}

void RA_ObjectList_set(RA_ObjectList *obj_list, uint32_t idx, RA_Object *new_obj) {
  assert(obj_list != NULL);

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
  anim_list->count = 0;
  anim_list->capacity = DA_INIT_SIZE;
  anim_list->animations = malloc(DA_INIT_SIZE * sizeof(RA_Animation));
  assert(anim_list->animations != NULL);
}

void RA_AnimationList_push(RA_AnimationList *anim_list, RA_Animation *new_anim) {
  assert(anim_list != NULL);

  if (anim_list->count == anim_list->capacity) {
    anim_list->capacity *= 2;
    assert(realloc(anim_list->animations, anim_list->capacity * sizeof(RA_Animation)) != NULL);
  }

  anim_list->animations[anim_list->count] = new_anim;
  anim_list->count++;
}

RA_Animation *RA_AnimationList_pop(RA_AnimationList *anim_list) {
  assert(anim_list != NULL);

  if (anim_list->count == 0) return NULL;

  return anim_list->animations[--anim_list->count];
}

RA_Animation *RA_AnimationList_popFirst(RA_AnimationList *anim_list) {
  assert(anim_list != NULL);

  if (anim_list->count == 0) return NULL;

  RA_Animation *first_anim = anim_list->animations[0];

  for (uint32_t i = 1; i < anim_list->count; i++)
    anim_list->animations[i - 1] = anim_list->animations[i];

  anim_list->count--;

  return first_anim;
}

RA_Animation *RA_AnimationList_getAt(RA_AnimationList *anim_list, uint32_t idx) {
  assert(anim_list != NULL);

  if (anim_list->count <= idx) return NULL;

  return anim_list->animations[idx];
}

void RA_AnimationList_set(RA_AnimationList *anim_list, uint32_t idx, RA_Animation *new_anim) {
  assert(anim_list != NULL);

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

void RA_Object_initEmpty(RA_Object *obj) {
  RA_Object_init(obj, (Vector2){0, 0}, RA_Object_emptyRender);
}

void RA_Object_emptyRender(void *self) {
  (void)self;
}

void RA_Animation_init(RA_Animation *anim,
                       RA_Object *obj,
                       float duration,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float)) {
  anim->_id = ++animation_id;
  anim->object = obj;
  anim->duration = duration;
  anim->elapsed_time = 0.0f;
  anim->update = update;
  anim->interpolate = interpolate;
}

void RA_Animation_defaultInit(RA_Animation *anim,
                              RA_Object *obj,
                              float duration,
                              void (*interpolate)(void *, float)) {
  RA_Animation_init(anim, obj, duration, RA_Animation_defaultUpdate, interpolate);
}

bool RA_Animation_defaultUpdate(void *self, float dt) {
  RA_Animation *anim = (RA_Animation *)self;

  anim->elapsed_time += dt;
  anim->interpolate(anim, fminf(anim->elapsed_time / anim->duration, 1.0f));

  bool completed = anim->elapsed_time >= anim->duration;
  if (completed) anim->elapsed_time = 0.0f;

  return completed;
}

void RA_Scene_init(RA_Scene *scene, const char *title, int width, int height, Color color) {
  scene->current_animation = NULL;
  scene->color = color;
  scene->width = width;
  scene->height = height;
  scene->title = title;

  RA_ObjectList_init(&scene->object_list);
  RA_AnimationList_init(&scene->animation_list);
}

void RA_Scene_play(RA_Scene *scene, RA_Animation *anim) {
  assert((scene != NULL) && (anim != NULL));

  RA_AnimationList_push(&scene->animation_list, anim);
}

void RA_Scene_render(RA_Scene *scene) {
  BeginDrawing();

  ClearBackground(scene->color);
  for (uint32_t i = 0; i < scene->object_list.count; i++) {
    RA_Object *obj = RA_ObjectList_getAt(&scene->object_list, i);
    if (obj != NULL) obj->render(obj);
  }

  EndDrawing();
}

void RA_Scene_update(RA_Scene *scene, float dt) {
  if ((scene->current_animation == NULL) && (scene->animation_list.count > 0)) {
    scene->current_animation = RA_AnimationList_popFirst(&scene->animation_list);
    RA_Object *current_obj = scene->current_animation->object;
    TraceLog(LOG_INFO, "started animation #%i", scene->current_animation->_id);
    // if (!RA_ObjectList_contains(&scene->object_list, current_obj))
    RA_ObjectList_push(&scene->object_list, current_obj);
  }

  if ((scene->current_animation != NULL) &&
      (scene->current_animation->update(scene->current_animation, dt))) {
    TraceLog(LOG_INFO, "finished animation #%i", scene->current_animation->_id);
    scene->current_animation = NULL;
  }
}

void RA_Scene_destroy(RA_Scene *scene) {
  RA_ObjectList_destroy(&scene->object_list);
  RA_AnimationList_destroy(&scene->animation_list);
  scene = NULL;
}

void playScene(RA_Scene *scene) {
  InitWindow(scene->width, scene->height, scene->title);
  SetTargetFPS(60);

  float last_time = GetTime();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q)) break;

    float current_time = GetTime();
    float dt = current_time - last_time;
    last_time = current_time;

    RA_Scene_update(scene, dt);
    RA_Scene_render(scene);
  }

  CloseWindow();
}

void recordScene(RA_Scene *scene) {
  // TODO: will be implemented later
  scene = scene;
}

// ------------------------------ Built-In RA_Objects & RA_Animations ------------------------------

// --------------- RA_Circle ---------------

void RA_Circle_init(RA_Circle *circle,
                    Vector2 center,
                    float radius,
                    float outline_thickness,
                    int segments,
                    Color circle_color,
                    Color outline_color,
                    void (*render)(void *)) {
  RA_Object_init(&circle->base, center, render);
  circle->radius = radius;
  circle->angle = 0.0f;
  circle->outline_thickness = outline_thickness;
  circle->segments = segments;
  circle->circle_color = circle_color;
  circle->outline_color = outline_color;
}

void RA_Circle_defaultInit(RA_Circle *circle, Vector2 center, float radius) {
  RA_Circle_init(circle, center, radius, 25.0f, 100, BLUE, DARKBLUE, RA_Circle_defaultRender);
}

void RA_Circle_defaultRender(void *self) {
  RA_Circle *circle = (RA_Circle *)self;

  float inner_radius = circle->radius - circle->outline_thickness / 2;
  float outer_radius = circle->radius + circle->outline_thickness / 2;
  DrawRing(circle->base.position,
           inner_radius,
           outer_radius,
           0.0f,
           circle->angle,
           circle->segments,
           circle->outline_color);
}

void RA_Circle_fillInnerRender(void *self) {
  RA_Circle *circle = (RA_Circle *)self;

  float half_thickness = circle->outline_thickness / 2;

  DrawCircleSector(circle->base.position,
                   circle->radius + half_thickness,
                   0.0f,
                   circle->angle,
                   circle->segments,
                   circle->outline_color);

  DrawCircleSector(circle->base.position,
                   circle->radius - half_thickness,
                   0.0f,
                   circle->angle,
                   circle->segments,
                   circle->circle_color);
}

void RA_CircleAnimation_init(RA_Animation *anim,
                             RA_Circle *circle,
                             float duration,
                             bool (*update)(void *, float),
                             void (*interpolate)(void *, float)) {
  RA_Animation_init(anim, (RA_Object *)circle, duration, update, interpolate);
}

void RA_CircleAnimation_defaultInit(RA_Animation *anim, RA_Circle *circle) {
  RA_CircleAnimation_init(
      anim, circle, 0.7f, RA_Animation_defaultUpdate, RA_CircleAnimation_defaultInterpolate);
}

void RA_CircleAnimation_defaultInterpolate(void *self, float time) {
  RA_Animation *anim = (RA_Animation *)self;
  RA_Circle *circle = (RA_Circle *)anim->object;
  circle->angle = time * 360.0f;
}

// --------------- RA_Circle ---------------

// ---------------- RA_Wait ----------------

static RA_Object wait_object;

void RA_WaitAnimation_init(RA_Animation *anim, float duration) {
  RA_Object_initEmpty(&wait_object);
  RA_Animation_init(
      anim, &wait_object, duration, RA_Animation_defaultUpdate, RA_WaitAnimation_interpolate);
}

void RA_WaitAnimation_interpolate(void *self, float time) {
  (void)time;
  (void)self;
}

// ---------------- RA_Wait ----------------
