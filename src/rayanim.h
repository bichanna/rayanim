#ifndef RAYANIM_H
#define RAYANIM_H

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define DA_INIT_SIZE 12

typedef struct RA_Scene RA_Scene;

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
  bool done;

  bool (*update)(void *, float);
  void (*interpolate)(void *, float);
  void (*push_to_object_list)(RA_Scene *);
} RA_Animation;

typedef struct RA_AnimationList {
  RA_Animation **animations;
  uint32_t count;
  uint32_t capacity;
} RA_AnimationList;

struct RA_Scene {
  RA_ObjectList object_list;
  RA_AnimationList animation_list;
  RA_Animation *current_animation;
  Color color;
  const char *title;
  int width;
  int height;
};

typedef struct RA_TextureList {
  Texture *textures;
  uint32_t count;
  uint32_t capacity;
} RA_TextureList;

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

void RA_TextureList_init(RA_TextureList *texture_list);
void RA_TextureList_push(RA_TextureList *texture_list, Texture new_texture);
void RA_TextureList_unloadAll(RA_TextureList *texture_list);
void RA_TextureList_destroy(RA_TextureList *texture_list);

void RA_Object_init(RA_Object *obj, Vector2 position, void (*render)(void *));
void RA_Object_initEmpty(RA_Object *obj);
void RA_Object_emptyRender(void *self);

void RA_Animation_init(RA_Animation *anim,
                       RA_Object *obj,
                       float duration,
                       bool (*update)(void *, float),
                       void (*interpolate)(void *, float),
                       void (*push_to_object_list)(RA_Scene *));
void RA_Animation_defaultInit(RA_Animation *anim,
                              RA_Object *obj,
                              float duration,
                              void (*interpolate)(void *, float));
bool RA_Animation_defaultUpdate(void *self, float dt);
void RA_Animation_defaultPushToObjectList(RA_Scene *scene);

void RA_Scene_init(RA_Scene *scene, const char *title, int width, int height, Color color);
void RA_Scene_defaultInit(RA_Scene *scene, const char *title);
void RA_Scene_play(RA_Scene *scene, RA_Animation *anim);
void RA_Scene_play_these(RA_Scene *scene, RA_Animation **anims, int anim_count);
void RA_Scene_render(RA_Scene *scene);
void RA_Scene_update(RA_Scene *scene, float dt);
void RA_Scene_destroy(RA_Scene *scene);

void startScene(RA_Scene *scene);
void recordScene(RA_Scene *scene);

// ------------------------------ Built-In RA_Objects & RA_Animations ------------------------------

// --------------- RA_Circle ---------------

typedef struct RA_Circle {
  RA_Object base;
  float radius;
  float outline_thickness;
  int segments;
  Color inner_color;
  Color outline_color;

  float angle;
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
  Color inner_color;
  Color outline_color;

  float fst_quarter;
  float snd_quarter;
  float thrd_quarter;
  float last_quarter;
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

// -------------- RA_Square ----------------

void RA_Square_init(RA_Rectangle *square,
                    Vector2 position,
                    float length,
                    float outline_thickness,
                    Color inner_color,
                    Color outline_color,
                    void (*render)(void *));
void RA_Square_defaultInit(RA_Rectangle *square, Vector2 position, float length);
RA_Rectangle RA_Square_create(Vector2 position, float length);

// -------------- RA_Square ----------------

// ---------------- RA_Wait ----------------

void RA_WaitAnimation_init(RA_Animation *anim, float duration);
RA_Animation RA_WaitAnimation_create(float duration);
void RA_WaitAnimation_interpolate(void *self, float time);

// ---------------- RA_Wait ----------------

// -------------- RA_Disappear -------------

void RA_DisappearAnimation_init(RA_Animation *anim,
                                RA_Object *obj,
                                float duration,
                                bool (*update)(void *, float),
                                void (*interpolate)(void *, float));
void RA_DisappearAnimation_defaultInit(RA_Animation *anim, RA_Object *obj);
RA_Animation RA_DisappearAnimation_create(RA_Object *obj);
void RA_DisappearAnimation_defaultInterpolate(void *self, float time);
void RA_DisappearAnimation_defaultRender(void *self);

// -------------- RA_Disappear -------------

// ---------------- RA_Sync ----------------

typedef struct RA_SyncAnimation {
  RA_Animation base;
  RA_Animation **animations;
  uint8_t anim_count;
} RA_SyncAnimation;

void RA_SyncAnimation_init(RA_SyncAnimation *anim,
                           RA_Animation **anims,
                           uint8_t anim_count,
                           void (*push_to_object_list)(RA_Scene *));
void RA_SyncAnimation_defaultInit(RA_SyncAnimation *anim, RA_Animation **anims, uint8_t anim_count);
RA_SyncAnimation RA_SyncAnimation_create(RA_Animation **anims, uint8_t anim_count);
bool RA_SyncAnimation_defaultUpdate(void *self, float dt);
void RA_SyncAnimation_defaultInterpolate(void *self, float time);
void RA_SyncAnimation_defaultPushToObjectList(RA_Scene *scene);

// ---------------- RA_Sync ----------------

// ---------------- RA_Move ----------------

typedef struct RA_MoveAnimation {
  RA_Animation base;
  RA_Animation *target_anim;
  Vector2 initial_position;
  Vector2 target_position;
} RA_MoveAnimation;

void RA_MoveAnimation_init(RA_MoveAnimation *anim,
                           RA_Animation *target_anim,
                           float duration,
                           Vector2 target_pos,
                           bool (*update)(void *, float),
                           void (*interpolate)(void *, float),
                           void (*push_to_object_list)(RA_Scene *));
void RA_MoveAnimation_defaultInit(RA_MoveAnimation *anim,
                                  RA_Animation *target_anim,
                                  Vector2 target_pos);
RA_MoveAnimation RA_MoveAnimation_create(RA_Animation *target_anim, Vector2 target_pos);
bool RA_MoveAnimation_defaultUpdate(void *self, float dt);
void RA_MoveAnimation_defaultPushToObjectList(RA_Scene *scene);

// ---------------- RA_Move ----------------

// ---------------- RA_Text ----------------

typedef struct RA_Text {
  RA_Object base;
  Font font;
  Color tint;
  float spacing;
  float font_size;
  char *full_text;
  float char_reveal_time;
  size_t display_char_count;
  float _elapsed_time;
} RA_Text;

void RA_Text_init(RA_Text *text,
                  char *full_text,
                  Font font,
                  Color tint,
                  float char_reveal_time,
                  float font_size,
                  float spacing,
                  Vector2 pos,
                  void (*render)(void *));
void RA_Text_defaultInit(RA_Text *text, char *full_text, Vector2 pos);
RA_Text RA_Text_create(char *full_text, Vector2 pos);
void RA_Text_defaultRender(void *self);
void RA_TextAnimation_init(RA_Animation *anim,
                           RA_Text *text,
                           float duration,
                           bool (*update)(void *, float),
                           void (*interpolate)(void *, float));
void RA_TextAnimation_defaultInit(RA_Animation *anim, RA_Text *text);
RA_Animation RA_TextAnimation_create(RA_Text *text);
void RA_TextAnimation_defaultInterpolate(void *self, float time);

// ---------------- RA_Text ----------------

// --------------- RA_Image ----------------

typedef struct RA_Image {
  RA_Object base;
  uint32_t texture_idx;
  char *image_path;
  float scale;
  uint8_t alpha;
  Color tint;
} RA_Image;

void RA_Image_init(RA_Image *image,
                   char *image_path,
                   Vector2 pos,
                   float scale,
                   Color tint,
                   void (*render)(void *));
void RA_Image_defaultInit(RA_Image *image, char *image_path, Vector2 pos);
RA_Image RA_Image_create(char *image_path, Vector2 pos);
void RA_Image_defaultRender(void *self);
void RA_ImageAnimation_init(RA_Animation *anim,
                            RA_Image *image,
                            float duration,
                            bool (*update)(void *, float),
                            void (*interpolate)(void *, float));
void RA_ImageAnimation_defaultInit(RA_Animation *anim, RA_Image *image);
RA_Animation RA_ImageAnimation_create(RA_Image *image);
void RA_ImageAnimation_defaultInterpolate(void *self, float time);

// --------------- RA_Image ----------------

#endif  // RAYANIM_H
