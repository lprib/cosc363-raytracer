#include "scene.h"

#include "cone.h"
#include "plane.h"
#include "scene_object.h"
#include "sphere.h"
#include "vec3.h"

void create_scene(scene_t *scene) {
    // ss[0] = new_sphere((vec3_t){-5.0, 0.0, -90.0}, 15.0);
    scene_object_t ss0 = new_cone((vec3_t){-5.0, -15, -90.0}, 6.0, 5.0);
    ss0.color = (vec3_t){0, 0, 1};
    // ss0.shininess = 5.0;
    // ss0.refract_c = 1.0;
    // ss0.refractive_index = 0.8;
    // ss0.is_reflective = true;
    // ss0.reflect_c = 0.7;

    // scene_object_t ss0 = new_sphere((vec3_t) {-5, -10, -90}, 5.0);
    // ss0.color = (vec3_t){1, 0, 1};

    scene_object_t ss1 = new_sphere((vec3_t){5, 6, -40}, 3.0);
    ss1.color = (vec3_t){0, 1, 1};
    ss1.is_reflective = true;
    ss1.reflect_c = 0.8;
    ss1.is_transparent = false;
    ss1.transparent_c = 0.5;

    scene_object_t ss2 = new_sphere((vec3_t){5, 5, -70}, 4.0);
    ss2.color = (vec3_t){1, 0, 0};
    ss2.is_reflective = true;
    ss2.reflect_c = 0.8;

    scene_object_t ss3 = new_sphere((vec3_t){-2.0, -10.0, -70.0}, 5.0);
    ss3.color = (vec3_t){1, 1, 0};
    ss3.is_reflective = false;
    ss3.is_transparent = true;
    ss3.transparent_c = 1.0;
    ss3.is_refractive = true;
    ss3.refract_c = 1.0;
    ss3.refractive_index = 0.95;
    ss3.reflect_c = 0.7;

    scene_object_t ss4 = new_plane4(
        (vec3_t){-20, -15, -40},
        (vec3_t){20, -15, -40},
        (vec3_t){20, -15, -200},
        (vec3_t){-20, -15, -200},
        false,
        false);
    ss4.color = (vec3_t){1, 1, 0};
    ss4.is_specular = false;

    scene_object_t ss5 = new_plane4(
        (vec3_t){-20, -15, -200},
        (vec3_t){20, -15, -200},
        (vec3_t){20, 15, -200},
        (vec3_t){-20, 15, -200},
        false,
        true
    );
    ss5.color = (vec3_t){1, 0, 0};

    // scene_object_t ss5 = new_plane4(
    //     (vec3_t){}
    // )

    add_object(scene, &ss0);
    add_object(scene, &ss1);
    // add_object(&scene, &ss2);
    // add_object(&scene, &ss3);
    add_object(scene, &ss4);
    add_object(scene, &ss5);
}