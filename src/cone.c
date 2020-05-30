#include "cone.h"

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
} cone_data_t;

double cone_intersect(scene_object_t *this, vec3_t p0, vec3_t dir) {
    cone_data_t *data = (cone_data_t *)this->data;
    double tangent = (data->r / data->h) * (data->r / data->h);

    double x_diff = p0.x - data->base.x;
    double z_diff = p0.z - data->base.z;
    double y_diff = data->h - p0.y + data->base.y;

    double a = (dir.x * dir.x) + (dir.z * dir.z) - tangent * (dir.y * dir.y);
    double b = (2.0 * x_diff * dir.x) + (2.0 * z_diff * dir.z) + (2.0 * tangent * y_diff * dir.y);
    float c = (x_diff * x_diff) + (z_diff * z_diff) - (tangent * y_diff * y_diff);

    double delta = b*b - 4*a*c;

    if(fabs(delta < 0.01)) {
        return -1;
    } else if(delta < 0.0) {
        return -1;
    }

    double t1 = (-b + sqrt(delta)) / (2 * a);
    double t2 = (-b - sqrt(delta)) / (2 * a);

    double final_t;

    if(t1 > t2) {
        final_t = t2;
    } else {
        final_t = t1;
    }

    double ray_y = p0.y + final_t * dir.y;

    if((ray_y > data->base.y) && (ray_y < data->base.y + data-> h)) {
        return final_t;
    } else {
        return -1;
    }
}

vec3_t cone_normal(scene_object_t *this, vec3_t pos) {
    cone_data_t *data = (cone_data_t *)this->data;
    double r = sqrt((pos.x - data->base.x) * (pos.x - data->base.x) + (pos.z - data->base.z) * (pos.z - data->base.z));
    vec3_t norm = (vec3_t) {pos.x - data->base.x, r * (data->r / data->h), pos.z - data->base.z};
    return normalize(norm);
}

void cone_desctruct(scene_object_t *this) { free(this->data); }

vec3_t cone_get_color(scene_object_t *this, vec3_t hit) {
    return this->color;
}

scene_object_t new_cone(vec3_t base, double r, double h) {
    cone_data_t *data = (cone_data_t *)malloc(sizeof(cone_data_t));
    data->base = base;
    data->r = r;
    data->h = h;

    scene_object_t n = default_scene_object();
    n.data = data;
    n.intersect = &cone_intersect;
    n.normal = &cone_normal;
    n.desctruct = &cone_desctruct;
    n.get_color = &cone_get_color;

    return n;
}