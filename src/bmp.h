#ifndef BMP_H
#define BMP_H

#include "vec3.h"

typedef struct {
    int width;
    int height;
    int channels;
    char* data;
} texture_t;

texture_t new_texture(char* filename);

texture_t free_texture(texture_t* tex);

vec3_t get_color_at(texture_t *tex, double s, double t);

#endif