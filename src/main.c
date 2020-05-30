#include <GL/freeglut.h>

#include "plane.h"
#include "ray.h"
#include "scene_object.h"
#include "sphere.h"
#include "vec3.h"

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 800;
const int MAX_STEPS = 5;
float XMIN;
float XMAX;
float YMIN;
float YMAX;

scene_t scene;

vec3_t trace(ray_t *ray, int step) {
    vec3_t bg_color = ZERO_VEC;
    vec3_t light_pos = {10, 40, -3};
    vec3_t color = ZERO_VEC;
    scene_object_t *object;

    closest_point(ray, scene);
    if (ray->index == -1)
        return bg_color;
    object = &scene.objects[ray->index];

    color = get_lighting(object, light_pos, negate(ray->dir), ray->hit);
    // color = object->color;
    vec3_t light_vec = subtract(light_pos, ray->hit);

    ray_t shadow_ray = new_ray(ray->hit, light_vec);
    closest_point(&shadow_ray, scene);

    if ((shadow_ray.index > -1) && (shadow_ray.distance < length(light_vec))) {
        scene_object_t* shadow_hit = &scene.objects[shadow_ray.index];
        double shadow_coef = shadow_hit->is_transparent ? shadow_hit->transparent_c : 0.2;
        color = scale(color, shadow_coef);
    }

    if(object->is_transparent) {
        ray_t internal_ray = new_ray(add(ray->hit, scale(ray->dir, 1.E-6)), ray->dir);
        closest_point(&internal_ray, scene);
        ray_t exit_ray = new_ray(add(internal_ray.hit, scale(ray->dir, 1.E-6)), ray->dir);
        vec3_t behind_color = trace(&exit_ray, step);
        color = add(color, scale(behind_color, object->transparent_c));
    }

    if (object->is_reflective && step < MAX_STEPS) {
        double rho = object->reflect_c;
        vec3_t normal_vec = object->normal(object, ray->hit);
        vec3_t reflected_dir = reflect(ray->dir, normal_vec);
        ray_t reflected_ray = new_ray(ray->hit, reflected_dir);
        vec3_t reflected_color = trace(&reflected_ray, step + 1);
        color = add(color, scale(reflected_color, rho));
    }

    if (object->is_refractive && step < MAX_STEPS) {
        vec3_t norm = object->normal(object, ray->hit);
        //maybe normalise incident

        vec3_t refract_direction = refract(normalize(ray->dir), normalize(norm), 1 / object->refractive_index);
        ray_t refract_ray = new_ray(ray->hit, refract_direction);
        closest_point(&refract_ray, scene);
        vec3_t exiting_normal = object->normal(object, refract_ray.hit);
        vec3_t exiting_refract_direction = refract(refract_direction, negate(exiting_normal), object->refractive_index);
        ray_t exiting_refreact_ray = new_ray(refract_ray.hit, exiting_refract_direction);

        vec3_t refracted_color = trace(&exiting_refreact_ray, step + 1);

        color = add(color, scale(refracted_color, object->refract_c));
    }

    return color;
}

void display() {
    double xp, yp;
    double cell_x = (XMAX - XMIN) / (double)NUMDIV;
    double cell_y = (YMAX - YMIN) / (double)NUMDIV;

    vec3_t eye = {0.0, 0.0, 0.0};

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);

    for (int i = 0; i < NUMDIV; i++) {
        xp = XMIN + i * cell_x;

        for (int j = 0; j < NUMDIV; j++) {
            yp = YMIN + j * cell_y;

            vec3_t dir = {xp + 0.5 * cell_x, yp + 0.5 * cell_y, -EDIST};

            ray_t ray = new_ray(eye, dir);

            vec3_t color = trace(&ray, 1);

            glColor3f(color.x, color.y, color.z);
            glVertex2f(xp, yp); //Draw each cell with its color value
            glVertex2f(xp + cell_x, yp);
            glVertex2f(xp + cell_x, yp + cell_y);
            glVertex2f(xp, yp + cell_y);
        }
    }

    glEnd();
    glFlush();
}

void initialize() {
    XMIN = -WIDTH * 0.5;
    XMAX = WIDTH * 0.5;
    YMIN = -HEIGHT * 0.5;
    YMAX = HEIGHT * 0.5;

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    scene_object_t ss[5] = {0};

    ss[0] = new_sphere((vec3_t){-5.0, 0.0, -90.0}, 15.0);
    ss[0].color = (vec3_t){0, 0, 1};
    // ss[0].shininess = 5.0;
    ss[0].is_reflective = true;
    ss[0].reflect_c = 0.8;

    ss[1] = new_sphere((vec3_t){10, 10, -60}, 3.0);
    ss[1].color = (vec3_t){0, 1, 1};
    ss[1].is_reflective = true;
    ss[1].reflect_c = 0.8;

    ss[2] = new_sphere((vec3_t){5, 5, -70}, 4.0);
    ss[2].color = (vec3_t){1, 0, 0};
    ss[2].is_reflective = true;
    ss[2].reflect_c = 0.8;

    ss[3] = new_sphere((vec3_t){5.0, -10.0, -60.0}, 5.0);
    ss[3].color = (vec3_t){0, 1, 0};
    ss[3].is_reflective = false;
    ss[3].is_transparent = true;
    ss[3].transparent_c = 0.95;
    // ss[3].is_refractive = true;
    // ss[3].refract_c = 1.0;
    // ss[3].refractive_index = 0.8;
    ss[3].reflect_c = 0.8;

    ss[4] = new_plane4(
        (vec3_t){-20, -15, -40},
        (vec3_t){20, -15, -40},
        (vec3_t){20, -15, -200},
        (vec3_t){-20, -15, -200});
    ss[4].is_specular = false;

    scene = new_scene(ss, sizeof(ss) / sizeof(scene_object_t));
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}