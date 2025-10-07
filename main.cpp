#include "common.h"
#include "sceneObjects.h"
#include "components/sphere.h"
#include "components/camera.h"
#include "renderer.h"
#include "components/materials.h"
#include "vec3.h"

using namespace std;

int main() {

    sceneObjects world;

    auto material_ground = make_shared<lambertian>(color(0.0, 0.5, 0.8));
    auto material_center = make_shared<lambertian>(color(0.1, 0.7, 0.2));
    auto material_left = make_shared<dieletric>(1.5);
    auto material_right = make_shared<metal>(color(0.9, 0.7, 0.0), 0.0);

    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));

    auto material_bubble = make_shared<dieletric>(1.0/ 1.5);
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));

    camera cam;
    
    cam.vfov = 30;
    cam.center = point3(-2, 2, 1);
    cam.defocus_angle = 10.0;
    cam.focus_distance = length(point3(-2, 2, 1) - point3(0, 0, -1));

    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 100;
    rend.max_depth = 50;

    rend.render(world, cam);

}

