#include "scene.h"

#include "cone.h"
#include "plane.h"
#include "scene_object.h"
#include "sphere.h"
#include "vec3.h"
#include "colors.h"

#include <math.h>

//create cube at coords with size and color
void add_cube(scene_t* scene, double x, double y, double z, double size, vec3_t color) {
    double s = size;
    double r2 = 1.0 / sqrt(2);
    double reflect_c = 0.5;

    scene_object_t left = new_plane4(
        (vec3_t){x, y+s, z},
        (vec3_t){x-r2*s, y+s, z-r2*s},
        (vec3_t){x-r2*s, y, z-r2*s},
        (vec3_t){x, y, z},
        false,
        false
    );
    left.color = color;
    left.is_reflective = true;
    left.reflect_c = reflect_c;

    scene_object_t right = new_plane4(
        (vec3_t){x+r2*s, y+s, z-r2*s},
        (vec3_t){x, y+s, z},
        (vec3_t){x, y, z},
        (vec3_t){x+r2*s, y, z-r2*s},
        false,
        false
    );
    right.color = color;
    right.is_reflective = true;
    right.reflect_c = reflect_c;

    scene_object_t top = new_plane4(
        (vec3_t){x, y+s, z},
        (vec3_t){x+r2*s, y+s, z-r2*s},
        (vec3_t){x, y+s, z-2*r2*s},
        (vec3_t){x-r2*s, y+s, z-r2*s},
        false,
        false
    );
    top.color = color;
    top.is_reflective = true;
    top.reflect_c = reflect_c;

    add_object(scene, &left);
    add_object(scene, &right);
    add_object(scene, &top);
}

//populate scene with scene_object_t's
void create_scene(scene_t *scene) {
    scene_object_t bottom_plane = new_plane4(
        (vec3_t){-30, -15, 0},
        (vec3_t){30, -15, 0},
        (vec3_t){30, -15, -200},
        (vec3_t){-30, -15, -200},
        true,
        false);
    bottom_plane.is_specular = false;

    scene_object_t back_plane = new_plane4(
        (vec3_t){-30, -15, -130},
        (vec3_t){30, -15, -130},
        (vec3_t){30, 15, -130},
        (vec3_t){-30, 15, -130},
        false,
        true
    );
    back_plane.is_specular = false;

    scene_object_t shadow_sphere1 = new_sphere((vec3_t){-10, -13, -80}, 1);
    shadow_sphere1.color = BLUE;
    shadow_sphere1.is_reflective = true;

    scene_object_t shadow_sphere2 = new_sphere((vec3_t){-10, -10.8, -80}, 1);
    shadow_sphere2.color = RED;
    shadow_sphere2.is_reflective = true;

    scene_object_t shadow_sphere3 = new_sphere((vec3_t){-10, -8.6, -80}, 1);
    shadow_sphere3.color = WHITE;
    shadow_sphere3.is_reflective = true;

    scene_object_t sphere1 = new_sphere((vec3_t){10, -13, -80}, 1);
    sphere1.color = BLUE;
    sphere1.is_reflective = true;

    scene_object_t sphere2 = new_sphere((vec3_t){10, -10.8, -80}, 1);
    sphere2.color = RED;
    sphere2.is_reflective = true;

    scene_object_t sphere3 = new_sphere((vec3_t){10, -8.6, -80}, 1);
    sphere3.color = WHITE;
    sphere3.is_reflective = true;

    scene_object_t refract_sphere = new_sphere((vec3_t){0, -6, -60}, 5);
    refract_sphere.is_transparent = true;
    refract_sphere.transparent_c = 1.0;
    refract_sphere.is_refractive = true;
    refract_sphere.refractive_index = 0.99;

    scene_object_t cone = new_cone((vec3_t){0, -15, -60}, 3, 4);
    cone.color = YELLOW;

    scene_object_t transparent_sphere1 = new_sphere((vec3_t){-5, -12.5, -50}, 1.5);
    transparent_sphere1.color = GREEN;
    transparent_sphere1.is_transparent = true;
    transparent_sphere1.transparent_c = 0.9;
    transparent_sphere1.is_reflective = true;
    transparent_sphere1.reflect_c = 0.1;

    scene_object_t transparent_sphere2 = new_sphere((vec3_t){5, -12.5, -50}, 1.5);
    transparent_sphere2.color = BLUE;
    transparent_sphere2.is_transparent = true;
    transparent_sphere2.transparent_c = 0.9;
    transparent_sphere2.is_reflective = true;
    transparent_sphere2.reflect_c = 0.1;

    scene_object_t reflect_sphere1 = new_sphere((vec3_t){-8, -12.5, -45}, 1.5);
    reflect_sphere1.color = BLUE;
    reflect_sphere1.is_reflective = true;
    reflect_sphere1.reflect_c = 1;

    scene_object_t reflect_sphere2 = new_sphere((vec3_t){8, -12.5, -45}, 1.5);
    reflect_sphere2.color = GREEN;
    reflect_sphere2.is_reflective = true;
    reflect_sphere2.reflect_c = 1;

    add_cube(scene, 0, -15, -45, 2, BLUE);
    add_cube(scene, 2, -15, -43, 1.4, GREEN);
    add_cube(scene, 3.4, -15, -41.6, 0.7, YELLOW);

    add_object(scene, &bottom_plane);
    add_object(scene, &back_plane);
    add_object(scene, &shadow_sphere1);
    add_object(scene, &shadow_sphere2);
    add_object(scene, &shadow_sphere3);
    add_object(scene, &sphere1);
    add_object(scene, &sphere2);
    add_object(scene, &sphere3);
    add_object(scene, &refract_sphere);
    add_object(scene, &cone);
    add_object(scene, &transparent_sphere1);
    add_object(scene, &transparent_sphere2);
    add_object(scene, &reflect_sphere1);
    add_object(scene, &reflect_sphere2);
}