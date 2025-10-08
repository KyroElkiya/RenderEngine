#include "common.h"
#include "sceneObjects.h"
#include "components/sphere.h"
#include "components/camera.h"
#include "renderer.h"
#include "components/materials.h"
#include "vec3.h"
#include "components/bvh.h"

using namespace std;

int main() {

    sceneObjects world;

    auto ground_material = make_shared<lambertian>(color(0.7, 0.7, 0.7));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if (length(center - point3(4, 0.2, 0)) > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = make_shared<dieletric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dieletric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1, material2));

    auto material3 = make_shared<metal>(color(0.85, 0.64, 0.125), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1, material3));
    
    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    
    cam.vfov = 20;
    cam.center = point3(13, 2, 3);
    cam.defocus_angle = 0.6;
    cam.focus_distance = length(cam.center - point3(0, 1, 0));

    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 100;
    rend.max_depth = 50;

    rend.render(world, cam);

}

