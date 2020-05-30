#include "vec3.h"

#include <math.h>

vec3_t add(vec3_t a, vec3_t b) {
    return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3_t subtract(vec3_t a, vec3_t b) {
    return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3_t negate(vec3_t n) { return subtract(ZERO_VEC, n); }

vec3_t normalize(vec3_t n) {
    double magnitude = length(n);
    return (vec3_t){n.x / magnitude, n.y / magnitude, n.z / magnitude};
}

double dot(vec3_t a, vec3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3_t scale(vec3_t n, double scl) {
    return (vec3_t){scl * n.x, scl * n.y, scl * n.z};
}

vec3_t reflect(vec3_t incident, vec3_t norm) {
    norm = normalize(norm);
    double coef = 2 * dot(norm, incident);
    vec3_t scaled = scale(norm, coef);
    return subtract(incident, scaled);
}

double length(vec3_t n) {
    return sqrt(dot(n, n));
}

vec3_t cross(vec3_t a, vec3_t b) {
    return (vec3_t){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

vec3_t refract(vec3_t incident, vec3_t norm, double eta) {
    double k = 1.0 - eta * eta * (1.0 - dot(norm, incident) * dot(norm, incident));
    if (k < 0.0) {
        return ZERO_VEC;
    } else {
        return subtract(scale(incident, eta), scale(norm, eta * dot(norm, incident) + sqrt(k)));
    }
}