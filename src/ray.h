#ifndef RAY_H
#define RAY_H

#include "scene_object.h"
#include "vec3.h"

typedef struct {
    vec3_t p0;
    vec3_t dir;
    vec3_t hit;
    int index;
    double distance;
} ray_t;

ray_t default_ray();

ray_t new_ray(vec3_t source, vec3_t direction);

void closest_point(ray_t *this, scene_t scene);

#endif