#ifndef JULIA_H
#define JULIA_H

#include "vec3.h"

void populate_buffer();

vec3_t get_julia_color_st(double s, double t);

#endif