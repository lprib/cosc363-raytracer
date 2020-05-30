#ifndef PLANE_H
#define PLANE_H

#include "scene_object.h"
#include "vec3.h"

scene_object_t new_plane4(vec3_t pa, vec3_t pb, vec3_t pc, vec3_t pd);

scene_object_t new_plane3(vec3_t pa, vec3_t pb, vec3_t pc);

#endif