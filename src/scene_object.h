#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "stdbool.h"
#include "vec3.h"

typedef struct scene_object_s {
    vec3_t color;
    bool is_reflective;
    bool is_refractive;
    bool is_specular;
    bool is_transparent;
    double reflect_c;
    double refract_c;
    double transparent_c;
    double refractive_index;
    double shininess;
    void *data;
    // V-Table for polymorphism of different shapes:
    double (*intersect)(struct scene_object_s *this, vec3_t p0, vec3_t dir);
    vec3_t (*normal)(struct scene_object_s *this, vec3_t pos);
    void (*desctruct)(struct scene_object_s *this);
    vec3_t (*get_color)(struct scene_object_s *this, vec3_t hit);
} scene_object_t;

typedef struct {
    scene_object_t *objects;
    int length;
} scene_t;

scene_object_t default_scene_object();

vec3_t get_lighting(scene_object_t *object, vec3_t* light_pos, int num_lights, vec3_t view_vec, vec3_t hit, vec3_t object_color);

scene_t new_scene();

// Add new_obj to scene. Note new_obj reference does not need to outlive scene reference, as the object is memcpy'd into the scene.
void add_object(scene_t* scene, scene_object_t* new_obj);

#endif