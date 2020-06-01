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
    double a = pow(dir.x, 2) + pow(dir.z, 2);
    double b = 2 * (dir.x * (p0.x - data->base.x) + dir.z * (p0.z - data->base.z));
    double c = pow(p0.x - data->base.x, 2) + pow(p0.z - data->base.z, 2) - (data->r * data->r);

    double delta = b * b - 4 * a * c;
    if (fabs(delta) < 1.0E-4) {
        return -1;
    }

    if (delta <= 0.0) {
        return -1.0;
    }

    double t1 = (-b + sqrt(delta)) / (2 * a);
    double t2 = (-b - sqrt(delta)) / (2 * a);

    if(t1 > t2) {
        double temp = t1;
        t1 = t2;
        t2 = temp;
    }

    vec3_t t1_hit = add(p0, scale(dir, t1));
    vec3_t t2_hit = add(p0, scale(dir, t1));

    if(t1_hit.y > data->h && t2_hit.y < data->h) {
        return (data->h - p0.y) / dir.y;
    }
    // if (t1_hit.y - data->base.y > data->h || t1_hit.y - data->base.y < 0.0) {
    //     return -1;
    // }

    return t1;
}

vec3_t cyl_normal(scene_object_t *this, vec3_t pos) {
    cylinder_data_t *data = (cylinder_data_t *)this->data;
    vec3_t n = subtract(pos, data->base);
    n.y = 0.0;
    n = normalize(n);
    return n;
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