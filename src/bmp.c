#include "bmp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

texture_t new_texture(char *filename) {
    texture_t new_tex;

    char header1[18];
    char header2[18];
    short int planes;
    short int bpp;
    int width, height;
    int nbytes, size, index, temp;

    FILE *file;
    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("error opening file %s\n", filename);
        exit(1);
    }

    size_t ignore = 0;

    ignore = fread(header1, sizeof(header1), 1, file);
    ignore = fread(&width, sizeof(int), 1, file);
    ignore = fread(&height, sizeof(int), 1, file);
    ignore = fread(&planes, sizeof(short int), 1, file);
    ignore = fread(&bpp, sizeof(short int), 1, file);
    ignore = fread(header2, sizeof(header2), 1, file);

    nbytes = bpp / 8;
    size = width * height * nbytes;
    char *image_data = malloc(sizeof(char) * size);
    ignore = fread(image_data, size, 1, file);
    for (int i = 0; i < width * height; i++) {
        index = i * nbytes;
        temp = image_data[index];
        image_data[index] = image_data[index + 2];
        image_data[index + 2] = temp;
    }

    new_tex.width = width;
    new_tex.height = height;
    new_tex.channels = nbytes;
    new_tex.data = image_data;

    printf("Image %s loaded successfully\n", filename);

    return new_tex;
}

texture_t free_texture(texture_t *tex) {
    free(tex->data);
}

vec3_t get_color_at(texture_t *tex, double s, double t) {
    if (tex->width == 0 || tex->height == 0) {
        return ZERO_VEC;
    }

    int i = (int)(s * tex->width);
    int j = (int)(t * tex->height);
    if (i < 0 || i > tex->width - 1 || j < 0 || j > tex->height - 1) {
        return ZERO_VEC;
    }

    int index = ((j * tex->width) + i) * tex->channels;

    int r = tex->data[index];
    int g = tex->data[index + 1];
    int b = tex->data[index + 2];

    if (r < 0)
        r += 255;
    if (g < 0)
        g += 255;
    if (b < 0)
        b += 255;

    double rn = (double)r / 255.0;
    double gn = (double)g / 255.0;
    double bn = (double)b / 255.0;

    return (vec3_t){rn, gn, bn};
}
