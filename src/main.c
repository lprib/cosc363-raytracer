#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "ray.h"
#include "scene.h"
#include "scene_object.h"
#include "vec3.h"

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
int AA_FACTOR = 2;
const int MAX_STEPS = 20;
float XMIN;
float XMAX;
float YMIN;
float YMAX;

scene_t scene;

#define NUM_LIGHTS 2
vec3_t lights[] = {
    {20, 10, -20},
    {10, 2, -3}};

vec3_t spolight_location = {-5, -3.6, -60};
// gets normalized in initialize()
vec3_t spolight_direction = {-1, -1, -3};
double spotlight_angle = 0.2;

vec3_t fog_color = {0.0, 0.0, 0.0};
double fog_intensity = 210.0;

vec3_t trace(ray_t *ray, int step) {
    vec3_t bg_color = ZERO_VEC;
    vec3_t color = ZERO_VEC;
    scene_object_t *object;

    closest_point(ray, scene);
    if (ray->index == -1) {
        return fog_color;
    }
    object = &scene.objects[ray->index];

    // return object->get_color(object, ray->hit);

    color = get_lighting(object, lights, NUM_LIGHTS, negate(ray->dir), ray->hit, object->get_color(object, ray->hit));

    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3_t light_vec = subtract(lights[i], ray->hit);

        ray_t shadow_ray = new_ray(ray->hit, light_vec);
        closest_point(&shadow_ray, scene);

        if ((shadow_ray.index > -1) && (shadow_ray.distance < length(light_vec))) {
            scene_object_t *shadow_hit = &scene.objects[shadow_ray.index];
            if (shadow_hit->is_transparent) {
                double transparent_shadow_coef = 1.5;
                color = add(color, scale(shadow_hit->color, transparent_shadow_coef *(1 - shadow_hit->transparent_c)));
            } else {
                color = scale(color, 0.8);
            }
        }
    }

    if (object->is_transparent) {
        ray_t internal_ray = new_ray(ray->hit, ray->dir);
        closest_point(&internal_ray, scene);
        ray_t exit_ray = new_ray(internal_ray.hit, ray->dir);
        vec3_t behind_color = trace(&exit_ray, step);
        vec3_t color1 = lerp(color, behind_color, object->transparent_c);
        vec3_t color2 = add(color, scale(behind_color, object->transparent_c));
        color = lerp(color1, color2, 0.3);
    }

    if (object->is_refractive && step < MAX_STEPS) {
        vec3_t norm = object->normal(object, ray->hit);
        vec3_t refract_direction = refract(normalize(ray->dir), normalize(norm), 1 / object->refractive_index);
        ray_t refract_ray = new_ray(ray->hit, refract_direction);
        closest_point(&refract_ray, scene);
        vec3_t exiting_normal = object->normal(object, refract_ray.hit);
        vec3_t exiting_refract_direction = refract(refract_direction, negate(exiting_normal), object->refractive_index);
        ray_t exiting_refreact_ray = new_ray(refract_ray.hit, exiting_refract_direction);
        vec3_t refracted_color = trace(&exiting_refreact_ray, step + 1);

        color = lerp(color, refracted_color, object->refract_c);
    }

    if (object->is_reflective && step < MAX_STEPS) {
        vec3_t normal_vec = object->normal(object, ray->hit);
        vec3_t reflected_dir = reflect(ray->dir, normal_vec);
        ray_t reflected_ray = new_ray(ray->hit, reflected_dir);
        vec3_t reflected_color = trace(&reflected_ray, step + 1);
        color = add(color, scale(reflected_color, object->reflect_c));
    }

    //spotlight
    vec3_t hit_to_spotlight = subtract(spolight_location, ray->hit);
    double cos_angle_to_spotlight = dot(spolight_direction, negate(hit_to_spotlight)) / length(spolight_direction) / length(hit_to_spotlight);
    if (acos(cos_angle_to_spotlight) < spotlight_angle) {
        // hit is within spotlight boundary
        ray_t spotlight_shadow_ray = new_ray(ray->hit, hit_to_spotlight);
        closest_point(&spotlight_shadow_ray, scene);
        if ((spotlight_shadow_ray.index == -1) || (spotlight_shadow_ray.distance > length(hit_to_spotlight))) {
            color = add(color, (vec3_t){0.3, 0.3, 0.3});
        }
    }

    vec3_t fog_dist = (vec3_t){ray->hit.x * 3.5, ray->hit.y, ray->hit.z};
    double fog_scale = length(fog_dist) / fog_intensity;
    if (fog_scale > 1.0) {
        fog_scale = 1.0;
    }
    color = lerp(color, fog_color, fog_scale);

    return color;
}

void display() {
    double xp, yp, xsp, ysp;
    double cell_x = (XMAX - XMIN) / (double)(NUMDIV);
    double cell_y = (YMAX - YMIN) / (double)(NUMDIV);
    double subcell_x = cell_x / AA_FACTOR;
    double subcell_y = cell_y / AA_FACTOR;

    vec3_t eye = {0.0, -5, 0.0};

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);

    vec3_t buf[AA_FACTOR * AA_FACTOR];

    int progress_division = NUMDIV / 10;
    int percentage = 0;

    printf("Rendering...\n");

    for (int i = 0; i < NUMDIV; i++) {
        xp = XMIN + i * cell_x;

        for (int j = 0; j < NUMDIV; j++) {
            yp = YMIN + j * cell_y;

            for (int aa_x = 0; aa_x < AA_FACTOR; aa_x++) {
                for (int aa_y = 0; aa_y < AA_FACTOR; aa_y++) {
                    xsp = xp + aa_x * subcell_x;
                    ysp = yp + aa_y * subcell_y;
                    vec3_t dir = {xsp + 0.5 * subcell_x, ysp + 0.5 * subcell_y, -EDIST};
                    ray_t ray = new_ray(eye, dir);
                    vec3_t color = trace(&ray, 1);
                    buf[aa_y * AA_FACTOR + aa_x] = color;
                }
            }

            //average
            vec3_t sum = ZERO_VEC;
            for (int buf_idx = 0; buf_idx < AA_FACTOR * AA_FACTOR; buf_idx++) {
                sum = add(sum, buf[buf_idx]);
            }

            vec3_t color = scale(sum, 1.0 / (double)(AA_FACTOR * AA_FACTOR));

            glColor3f(color.x, color.y, color.z);
            glVertex2f(xp, yp); //Draw each cell with its color value
            glVertex2f(xp + cell_x, yp);
            glVertex2f(xp + cell_x, yp + cell_y);
            glVertex2f(xp, yp + cell_y);
        }

        if (i % progress_division == 0) {
            printf("%d%% complete.\n", percentage);
            percentage += 10;
        }
    }

    printf("100%% complete.\n");

    glEnd();
    glFlush();
}

void initialize() {
    XMIN = -WIDTH * 0.5;
    XMAX = WIDTH * 0.5;
    YMIN = -HEIGHT * 0.5;
    YMAX = HEIGHT * 0.5;

    spolight_direction = normalize(spolight_direction);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    scene = new_scene();
    create_scene(&scene);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");
    glutDisplayFunc(display);
    initialize();

    if (argc > 1) {
        int aa = atoi(argv[1]);
        if (aa > 0) {
            printf("Continuing with %dx anti-aliasing\n", aa);
            AA_FACTOR = aa;
        } else {
            printf("invalid anti-aliasing amount %s, continuing with 2x anti-aliasing\n", argv[1]);
            AA_FACTOR = 2;
        }
    } else {
        printf("No anti-aliasing amount specified, continuing with 2x anti-aliasing\n");
        AA_FACTOR = 2;
    }

    glutMainLoop();
    return 0;
}