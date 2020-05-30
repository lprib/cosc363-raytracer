#include "plane.h"
#include "scene_object.h"
#include "vec3.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

typedef struct {
    vec3_t a;
    vec3_t b;
    vec3_t c;
    vec3_t d;
    int nverts;
    texture_t tex;
} plane_data_t;

bool is_inside(scene_object_t *this, vec3_t q) {
    plane_data_t *d = (plane_data_t *)this->data;

    vec3_t n = this->normal(this, q);

    vec3_t ua = subtract(d->b, d->a);
    vec3_t ub = subtract(d->c, d->b);
    vec3_t uc = subtract(d->d, d->c);
    vec3_t ud = subtract(d->a, d->d);
    vec3_t va = subtract(q, d->a);
    vec3_t vb = subtract(q, d->b);
    vec3_t vc = subtract(q, d->c);
    vec3_t vd = subtract(q, d->d);

    double ka = dot(cross(ua, va), n);
    double kb = dot(cross(ub, vb), n);
    double kc = dot(cross(uc, vc), n);
    double kd;

    if (d->nverts == 4)
        kd = dot(cross(ud, vd), n);
    else
        kd = ka;
    if (ka > 0 && kb > 0 && kc > 0 && kd > 0)
        return true;
    if (ka < 0 && kb < 0 && kc < 0 && kd < 0)
        return true;
    else
        return false;
}

vec3_t plane_normal(scene_object_t *this, vec3_t pos) {
    plane_data_t *d = (plane_data_t *)this->data;
    vec3_t v1 = subtract(d->c, d->b);
    vec3_t v2 = subtract(d->a, d->b);
    vec3_t n = cross(v1, v2);
    n = normalize(n);
    return n;
}

double plane_intersect(scene_object_t *this, vec3_t p0, vec3_t dir) {
    plane_data_t *d = (plane_data_t *)this->data;
    vec3_t n = this->normal(this, p0);
    vec3_t v_dif = subtract(d->a, p0);
    double d_dot_n = dot(dir, n);
    if(fabs(d_dot_n) < 1.E-4) return -1;

    double t = dot(v_dif, n) / d_dot_n;
    if(fabs(t) < 1.E-4) return -1;

    vec3_t q = add(p0, scale(dir, t));
    if(is_inside(this, q)) {
        return t;
    } else {
        return -1;
    }
}

void plane_desctruct(scene_object_t *this) {
    plane_data_t *d = (plane_data_t *)this->data;
    free_texture(&d->tex);
    free(this->data);
    }

vec3_t plane_get_color(scene_object_t*this, vec3_t hit) {
    plane_data_t *d = (plane_data_t *)this->data;

    // return this->color;
    int stripe_width = 5;
    int iz = hit.z;
    int k = iz%2;

    vec3_t color = ZERO_VEC;
    if(k==0) {
        color = (vec3_t){0, 1, 0};
    } else {
        color = (vec3_t){1, 1, 0.5};
    }

    double x1 = -15, x2 = 5, z1 = -60, z2=-90;
    double coord_s = (hit.x - x1)/(x2-x1);
    double coord_t = (hit.z - z1)/(z2-z1);

    if(coord_s > 0 && coord_s < 1 && coord_t > 0 && coord_t < 1) {
        color = get_color_at(&d->tex, coord_s, coord_t);
    }

    return color;
}

scene_object_t new_plane_from_data(plane_data_t* data) {
    scene_object_t n = default_scene_object();
    n.data = data;
    n.intersect = &plane_intersect;
    n.normal = &plane_normal;
    n.desctruct = &plane_desctruct;
    n.get_color = &plane_get_color;

    data->tex = new_texture("Butterfly.bmp");

    return n;
}

scene_object_t new_plane4(vec3_t pa, vec3_t pb, vec3_t pc, vec3_t pd) {
    plane_data_t* data = (plane_data_t*) malloc(sizeof(plane_data_t));
    data->a = pa;
    data->b = pb;
    data->c = pc;
    data->d = pd;
    data->nverts = 4;

    return new_plane_from_data(data);
}

scene_object_t new_plane3(vec3_t pa, vec3_t pb, vec3_t pc) {
    plane_data_t* data = (plane_data_t*) malloc(sizeof(plane_data_t));
    data->a = pa;
    data->b = pb;
    data->c = pc;
    data->d = ZERO_VEC;
    data->nverts = 3;

    return new_plane_from_data(data);
}