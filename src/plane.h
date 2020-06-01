#ifndef PLANE_H
#define PLANE_H

#include "scene_object.h"
#include "vec3.h"

scene_object_t new_plane4(vec3_t pa, vec3_t pb, vec3_t pc, vec3_t pd, bool is_checked, bool is_julia);

scene_object_t new_plane3(vec3_t pa, vec3_t pb, vec3_t pc, bool is_checked, bool is_julia);

#endif