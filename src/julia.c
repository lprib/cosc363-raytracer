#include "julia.h"
#include "scene_object.h"
#include "vec3.h"
#include "colors.h"
#include <math.h>

#define MAX_ITERATIONS 100

#define BUF_WIDTH 4000
#define BUF_HEIGHT 4000

static double cx = 0.32;
static double cy = 0.5;

static double r = 1.5;

static double buffer[BUF_WIDTH * BUF_HEIGHT] = {0};

static double map(double input, double input_start, double input_end,
                  double output_start, double output_end) {
    return output_start +
           ((output_end - output_start) / (input_end - input_start)) *
               (input - input_start);
}

double get_julia_xy(int x, int y) {
    double zx = map(x, 0, BUF_WIDTH, -r, r);
    double zy = map(y, 0, BUF_HEIGHT, -r, r);

    // double smooth_color = exp(-sqrt(zx * zx + zy * zy));

    int iter = 0;

    while (zx * zx + zy * zy < r * r && iter < MAX_ITERATIONS) {
        double x_temp = zx * zx - zy * zy;
        zy = 2 * zx * zy + cy;
        zx = x_temp + cx;
        // smooth_color += exp(-sqrt(zx * zx + zy * zy));
        iter++;
    }

    return map(iter, 0, MAX_ITERATIONS, 1.0, 0.0);
}

vec3_t get_julia_color_st(double s, double t) {
    int x_idx = (int)(s * (double)BUF_WIDTH);
    int y_idx = (int)(t * (double)BUF_HEIGHT);
    //flip output
    // double c = buffer[x_idx * BUF_WIDTH + y_idx];
    double c = get_julia_xy(y_idx, x_idx);
    return lerp(WHITE, RED, c);
}