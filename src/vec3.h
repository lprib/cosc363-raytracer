#ifndef VEC3_H
#define VEC3_H

typedef struct {
    double x;
    double y;
    double z;
} vec3_t;

#define ZERO_VEC \
    (vec3_t) { 0.0, 0.0, 0.0 }

// element-wise addition
vec3_t add(vec3_t a, vec3_t b);

// element-wise subtraction
vec3_t subtract(vec3_t a, vec3_t b);

// negate all components (same as subtract(ZERO_VEC, n))
vec3_t negate(vec3_t n);

// normalize vector
vec3_t normalize(vec3_t n);

// standard dot product
double dot(vec3_t a, vec3_t b);

// scale vector by constant
vec3_t scale(vec3_t n, double scl);

// reflect incident vector about the normal
vec3_t reflect(vec3_t incident, vec3_t norm);

// calculate magnitude of vector
double length(vec3_t n);

// vector cross product
vec3_t cross(vec3_t a, vec3_t b);

// refract incident vector through norm, with eta = u1/u2
vec3_t refract(vec3_t incident, vec3_t norm, double eta);

// linearly interpolate between two vectors. at t = 0, returns a. at t = 1, returns b.
vec3_t lerp(vec3_t a, vec3_t b, double t);

// debug print vector elements
void print_vec(vec3_t n);

// create normalized color from byte colors (in range 0-255)
// also applies some scaling so that colors look better in final render
vec3_t byte_color(int r, int g, int b);

#endif