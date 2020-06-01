#ifndef JULIA_H
#define JULIA_H

#include "vec3.h"

// return color of julia set from normalized st coords
vec3_t get_julia_color_st(double s, double t);

#endif