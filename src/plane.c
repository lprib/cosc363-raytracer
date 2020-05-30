// #include "plane.h"
// #include "scene_object.h"
// #include "vec3.h"
// #include <stdbool.h>

// typedef struct {
//     vec3_t a;
//     vec3_t b;
//     vec3_t c;
//     vec3_t d;
//     int nverts;
// } plane_data_t;

// bool is_inside(scene_object_t* this, vec3_t q) {
//     plane_data_t* d = (plane_data_t*) this->data;

//     vec3_t n = this->normal(this, q);

// 	vec3_t ua = subtract(d->b, d->a);
//     vec3_t ub = subtract(d->c, d->b);
//     vec3_t uc = subtract(d->d, d->v);
//     vec3_t ud = subtract(d->a, d->d);
//     vec3_t va = subtract(q, d->a);
//     vec3_t vb = subtract(q, d->b);
//     vec3_t vc = subtract(q, d->c);
//     vec3_t vd = subtract(q, d->d);

// 	double ka = glm::dot(glm::cross(ua, va), n);
// 	double kb = glm::dot(glm::cross(ub, vb), n);
// 	double kc = glm::dot(glm::cross(uc, vc), n);
// 	double kd;
// 	if (nverts_ == 4)
// 		kd = glm::dot(glm::cross(ud, vd), n);
// 	else
// 		kd = ka;
// 	if (ka > 0 && kb > 0 && kc > 0 && kd > 0) return true;
// 	if (ka < 0 && kb < 0 && kc < 0 && kd < 0) return true;
// 	else return false;
// }


// double plane_intersect(scene_object_t *this, vec3_t p0, vec3_t dir) {
// }

// vec3_t plane_normal(scene_object_t *this, vec3_t pos) {
// }

// void plane_desctruct(scene_object_t *this) { free(this->data); }

// scene_object_t new_plane4(vec3_t pa, vec3_t pb, vec3_t pc, vec3_t pd) {

// }