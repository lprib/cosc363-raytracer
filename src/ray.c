#include "ray.h"

#include "scene_object.h"
#include "vec3.h"

ray_t default_ray() {
    return (ray_t){ZERO_VEC, (vec3_t){0.0, 0.0, -1.0}, ZERO_VEC, -1, 0.0};
}

ray_t new_ray(vec3_t source, vec3_t direction) {
    ray_t n = default_ray();
    n.p0 = source;
    n.dir = normalize(direction);
    return n;
}

void closest_point(ray_t *this, scene_t scene) {
    vec3_t point = ZERO_VEC;
    double t_min = 1.E+6;

    for (int i = 0; i < scene.length; i++) {
        double t =
            scene.objects[i].intersect(&scene.objects[i], this->p0, this->dir);
        if (t > 0) {
            point = add(this->p0, scale(this->dir, t));
            if (t < t_min) {
                this->hit = point;
                this->index = i;
                this->distance = t;
                t_min = t;
            }
        }
    }
}