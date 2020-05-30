#include "scene_object.h"
#include "stddef.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

scene_object_t default_scene_object() {
    return (scene_object_t){
        (vec3_t){1.0, 1.0, 1.0},
        false,
        false,
        true,
        false,
        0.8,
        0.8,
        0.8,
        1.0,
        50.0,
        NULL,
        NULL};
}

vec3_t get_lighting(scene_object_t *object, vec3_t light_pos, vec3_t view_vec, vec3_t hit) {
    double ambient_term = 0.2;
    double diffuse_term = 0.0;
    double specular_term = 0.0;

    vec3_t normal_vec = object->normal(object, hit);
    vec3_t light_vec = subtract(light_pos, hit);
    light_vec = normalize(light_vec);

    double l_dot_n = dot(light_vec, normal_vec);

    if (object->is_specular) {
        vec3_t reflect_vec = reflect(negate(light_vec), normal_vec);
        double r_dot_v = dot(reflect_vec, view_vec);
        if (r_dot_v > 0) specular_term = pow(r_dot_v, object->shininess);
    }

    vec3_t ambient_color = scale(object->color, ambient_term);
    vec3_t diffuse_color = scale(object->color, l_dot_n);
    vec3_t specular_color = scale((vec3_t){1.0, 1.0, 1.0}, specular_term);

    vec3_t color_sum = add(add(ambient_color, diffuse_color), specular_color);

    return color_sum;
}

scene_t new_scene(scene_object_t* objs, int len) {
    scene_object_t* buf = malloc(sizeof(scene_object_t) * len);
    memcpy(buf, objs, sizeof(scene_object_t) * len);
    return (scene_t) {buf, len};
}