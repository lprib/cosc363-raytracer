#include "scene_object.h"
#include "stddef.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

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
        NULL,
        NULL};
}

vec3_t get_lighting(scene_object_t *object, vec3_t *light_pos, int num_lights, vec3_t view_vec, vec3_t hit, vec3_t object_color) {
    double ambient_term = 0.2;
    double diffuse_term = 0.0;
    double specular_term = 0.0;

    vec3_t normal_vec = object->normal(object, hit);
    vec3_t color_sum = scale(object_color, ambient_term);

    for (int i = 0; i < num_lights; i++) {
        vec3_t light_vec = subtract(light_pos[i], hit);
        light_vec = normalize(light_vec);
        double l_dot_n = dot(light_vec, normal_vec);

        if (object->is_specular) {
            vec3_t reflect_vec = reflect(negate(light_vec), normal_vec);
            double r_dot_v = dot(reflect_vec, view_vec);
            if (r_dot_v > 0)
                specular_term = pow(r_dot_v, object->shininess);
        }

        vec3_t diffuse_color = scale(object_color, l_dot_n);
        vec3_t specular_color = scale((vec3_t){1.0, 1.0, 1.0}, specular_term);

        color_sum = add(color_sum, add(diffuse_color, specular_color));
    }

    return color_sum;
}

scene_t new_scene() {
    scene_object_t* x = (scene_object_t*)malloc(sizeof(scene_object_t));
    scene_t s = {x, 0};
    return s;
}

void add_object(scene_t* scene, scene_object_t* new_obj) {
    scene->objects = realloc(scene->objects, sizeof(scene_object_t) * (scene->length + 1));
    memcpy(&scene->objects[scene->length], new_obj, sizeof(scene_object_t));
    scene ->length++;
}