#include "sphere.h"

#include <math.h>
#include <stdlib.h>

#include "scene_object.h"
#include "vec3.h"

typedef struct {
    vec3_t center;
    double raduis;
} sphere_data_t;

double sphere_intersect(scene_object_t *this, vec3_t p0, vec3_t dir) {
    sphere_data_t *data = (sphere_data_t *)this->data;
    vec3_t center = data->center;
    double raduis = data->raduis;

    vec3_t v_dif = subtract(p0, center);
    double b = dot(dir, v_dif);
    double len = length(v_dif);
    double c = len * len - raduis * raduis;
    double delta = b * b - c;

    if (fabs(delta) < 0.001)
        return -1.0;

    if (delta < 0.0)
        return -1.0;

    double t1 = -b - sqrt(delta);
    double t2 = -b + sqrt(delta);

    if (fabs(t1) < 0.001) {
        if (t2 > 0) {
            return t2;
        } else {
            t1 = -1.0;
        }
    }

    if (fabs(t2) < 0.001) {
        t2 = -1.0;
    }

    return (t1 < t2) ? t1 : t2;
}

vec3_t sphere_normal(scene_object_t *this, vec3_t pos) {
    sphere_data_t *d = (sphere_data_t *)this->data;
    vec3_t n = subtract(pos, d->center);
    n = normalize(n);
    return n;
}

void sphere_desctruct(scene_object_t *this) { free(this->data); }

scene_object_t new_sphere(vec3_t center, double radius) {
    sphere_data_t *data = (sphere_data_t *)malloc(sizeof(sphere_data_t));
    data->center = center;
    data->raduis = radius;

    scene_object_t n = default_scene_object();
    n.data = data;
    n.intersect = &sphere_intersect;
    n.normal = &sphere_normal;
    n.desctruct = &sphere_desctruct;

    return n;
}