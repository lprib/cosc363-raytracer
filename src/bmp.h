#ifndef BMP_H
#define BMP_H

#include "vec3.h"

// texture structure
typedef struct {
    int width;
    int height;
    int channels;
    char* data;
} texture_t;

// load new BMP texture from filename
texture_t new_texture(char* filename);

// free texture object
texture_t free_texture(texture_t* tex);

// get texture color at st coordinates
vec3_t get_color_at(texture_t *tex, double s, double t);

#endif