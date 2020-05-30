#ifndef VEC3_H
#define VEC3_H

typedef struct {
    double x;
    double y;
    double z;
} vec3_t;

#define ZERO_VEC \
    (vec3_t) { 0.0, 0.0, 0.0 }

vec3_t add(vec3_t a, vec3_t b);
vec3_t subtract(vec3_t a, vec3_t b);
vec3_t negate(vec3_t n);
vec3_t normalize(vec3_t n);
double dot(vec3_t a, vec3_t b);
vec3_t scale(vec3_t n, double scl);
vec3_t reflect(vec3_t incident, vec3_t norm);
double length(vec3_t n);
vec3_t cross(vec3_t a, vec3_t b);
vec3_t refract(vec3_t incident, vec3_t norm, double eta);

#endif