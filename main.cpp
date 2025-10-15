#include "common.h"
#include "sceneObjects.h"
#include "components/sphere.h"
#include "components/camera.h"
#include "renderer.h"
#include "components/materials.h"
#include "vec3.h"
#include "optimisations/bvh.h"
#include"components/mesh.h"
#include "objLoader.h"

using namespace std;

int main() {

    sceneObjects world;

    auto material_ground = make_shared<lambertian>(color(0.0, 0.5, 0.8));
    auto material_center = make_shared<lambertian>(color(0.1, 0.7, 0.2));
    auto material_left = make_shared<dieletric>(1.5);
    auto material_right = make_shared<metal>(color(0.9, 0.7, 0.0), 0.0);


    shared_ptr<mesh> test_back_01 = std::make_shared<mesh>(material_center);
    shared_ptr<mesh> test_back_02 = std::make_shared<mesh>(material_right);
    shared_ptr<mesh> test_back_03 = std::make_shared<mesh>(material_left);
    shared_ptr<mesh> test_floor = std::make_shared<mesh>(material_ground);

    objLoader::load("/home/Andrew/Downloads/test_back_01.obj", test_back_01);
    objLoader::load("/home/Andrew/Downloads/test_back_02.obj", test_back_02);
    objLoader::load("/home/Andrew/Downloads/test_back_03.obj", test_back_03);
    objLoader::load("/home/Andrew/Downloads/test_floor.obj", test_floor);
    
    test_back_01->finalize();
    test_back_02->finalize();
    test_back_03->finalize();
    test_floor->finalize();
    
    world.add(test_back_01);
    world.add(test_back_02);
    world.add(test_back_03);
    world.add(test_floor);


    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    //world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(0, -101.5, -1), 100, material_ground));

    //auto material_bubble = make_shared<dieletric>(1.0/ 1.5);
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));

    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    
    cam.vfov = 90;
    //cam.center = point3(-2, 2, 1);
    cam.center = point3(0);
    cam.defocus_angle = 0;
    cam.focus_distance = length(point3(0) - point3(0, 0, -1));

    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 100;
    rend.max_depth = 50;

    rend.render(world, cam);

}

