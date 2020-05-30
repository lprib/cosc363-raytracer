#include "cylinder.h"
#include "scene_object.h"
#include "vec3.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    vec3_t base;
    double r;
    double h;
} cylinder_data_t;

double cyl_intersect(scene_object_t *this, vec3_t p0, vec3_t dir) {
    cylinder_data_t *data = (cylinder_data_t *)this->data;

    double p_to_base_x = (p0.x - data->base.x);
    double p_to_base_z = (p0.z - data->base.z);
    double a = dir.x * dir.x + dir.z * dir.z;
    double b = dir.x * p_to_base_x + dir.z * p_to_base_z;
    double c = p_to_base_x * p_to_base_x + p_to_base_z * p_to_base_z - data->r * data->r;

    double delta = b * b - 4 * a * c;
    // if (fabs(delta) < 0.001) {
    //     return -1;
    // }

    if (delta <= 0.0) {
        return -1.0;
    }

    double t1 = (-b + sqrt(delta)) / (2 * a);
    double t2 = (-b - sqrt(delta)) / (2 * a);

    if (t1 > 0) {
        if (t1 < t2)
            return t1;
        else if (t2 > 0)
            return t2;
        else
            return t1;
    } else if (t2 > 0) {
        return t2;
    } else {
        return -1.0f;
    }

    double final_t;

    if (t1 > t2) {
        final_t = t2;
    } else {
        final_t = t1;
    }

    double ray_y = p0.y + final_t * dir.y;

    if ((ray_y >= data->base.y) && (ray_y <= data->base.y + data->h)) {
        return final_t;
    } else {
        return -1;
    }
}

vec3_t cyl_normal(scene_object_t *this, vec3_t pos) {
    cylinder_data_t *data = (cylinder_data_t *)this->data;
    return normalize((vec3_t){(pos.x - data->base.x), 0, (pos.z - data->base.z)});
}

void cyl_desctruct(scene_object_t *this) { free(this->data); }

vec3_t cyl_get_color(scene_object_t *this, vec3_t hit) {
    return this->color;
}

scene_object_t new_cylinder(vec3_t base, double r, double h) {
    cylinder_data_t *data = (cylinder_data_t *)malloc(sizeof(cylinder_data_t));
    data->base = base;
    data->r = r;
    data->h = h;

    scene_object_t n = default_scene_object();
    n.data = data;
    n.intersect = &cyl_intersect;
    n.normal = &cyl_normal;
    n.desctruct = &cyl_desctruct;
    n.get_color = &cyl_get_color;

    return n;
}