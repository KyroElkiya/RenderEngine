#include "scene/sceneObjects.h"
#include "scene/sphere.h"
#include "camera/camera.h"
#include "render/renderer.h"
#include "scene/bvh/bvh.h"
#include "scene/mesh.h"
#include "loader/objLoader.h"
#include "core/utils.h"
#include "material/lambertian.cpp"
#include "material/metallic.cpp"
#include "material/dielectric.cpp"
#include "material/emitter.cpp"

#include "debug/utils.h"

using namespace std;

int main() {
    
    print("working");
    sceneObjects world;


    auto material_ground = make_shared<lambertian>(color(0.0, 0.5, 0.8));
    auto material_center = make_shared<lambertian>(color(0.1, 0.7, 0.2));
    auto material_left   = make_shared<dieletric>(1.5);
    auto material_right  = make_shared<metal>(color(0.9, 0.7, 0.0), 0.0);
    auto material_chrome = make_shared<metal>(color(1, 1, 1), 0.0);

    auto material_dragon = make_shared<metal>(color(0.64, 0.34, 0.31), 0);

    auto material_emit = make_shared<emitter>(color(0.98, 0.98, 0.98), 50);
    world.add(make_shared<sphere>(point3(0, 2.5, 0), 0.5, material_emit));

    //print("Creating Shared Pointer");
    //shared_ptr<mesh> test_dragon = std::make_shared<mesh>(material_dragon);
    
    //print("Loading Mesh: Dragon");
    //objLoader::load("/home/Andrew/Downloads/dragon.obj", test_dragon);

    //print("Finalizing Mesh: Dragon");
    //test_dragon->finalize();

    //print("Adding Mesh: Dragon to world");
    //world.add(test_dragon);

    shared_ptr<mesh> dragon_floor = std::make_shared<mesh>(material_ground);
    objLoader::load("/home/Andrew/Downloads/floor_two_dragons.obj", dragon_floor);
    dragon_floor->finalize();
    world.add(dragon_floor);

    print("Creating Shared Pointers");
    shared_ptr<mesh> fg_dragon = std::make_shared<mesh>(material_center);
    shared_ptr<mesh> bg_dragon = std::make_shared<mesh>(material_center);

    
    print("Loading Dragon Meshes");
    objLoader::load("/home/Andrew/Downloads/dragon_high_fg.obj", fg_dragon);
    objLoader::load("/home/Andrew/Downloads/dragon_high_bg.obj", bg_dragon);

    
    print("Finalizing Dragon Meshes");
    fg_dragon->finalize();
    bg_dragon->finalize();

    print("Adding Dragons to World");
    world.add(fg_dragon);
    world.add(bg_dragon);


    shared_ptr<mesh> test_back_01 = std::make_shared<mesh>(material_center);
    shared_ptr<mesh> test_back_02 = std::make_shared<mesh>(material_left);
    shared_ptr<mesh> test_back_03 = std::make_shared<mesh>(material_right);
    shared_ptr<mesh> test_floor = std::make_shared<mesh>(material_ground);


    objLoader::load("/home/Andrew/Downloads/test_normals.obj", test_back_01);
    objLoader::load("/home/Andrew/Downloads/test_back_02.obj", test_back_02);
    objLoader::load("/home/Andrew/Downloads/test_back_03.obj", test_back_03);
    objLoader::load("/home/Andrew/Downloads/test_floor.obj", test_floor);
    
    // Assume mesh_ptr is your std::shared_ptr<mesh> holding the loaded OBJ
    //std::cout << "Vertices:\n";
    //for (size_t i = 0; i < test_back_01->getVertices().size(); ++i) {
    //    const auto& v = test_back_01->getVertices()[i];
    //    std::cout << "v" << i << ": " << v.x << " " << v.y << " " << v.z << "\n";
    //}

    //std::cout << "Triangles:\n";
    //for (size_t i = 0; i < test_back_01->getFaces().size(); ++i) {
    //    const auto& t = test_back_01->getFaces()[i];
    //    std::cout << "tri " << i << ": " 
    //          << t.vert_indices.x << " "
    //          << t.vert_indices.y << " "
    //          << t.vert_indices.z << "\n";
    //}

    test_back_01->finalize();
    test_back_02->finalize();
    test_back_03->finalize();
    test_floor->finalize();
    

    //world.add(test_back_01);
    //world.add(test_back_02);
    //world.add(test_back_03);
    //world.add(test_floor);


    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    //world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(0, -101.5, -1), 100, material_ground));

    //auto material_bubble = make_shared<dieletric>(1.0/ 1.5);
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));

    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    
    cam.vfov = 30;
    
    cam.center = point3(0);

    //cam.center = point3(0, 0.1, -1.3);
    //cam.lookat = point3(0, 0, -2);

    //cam.center = point3(-0.5, 0, -0.2);
    //cam.lookat = point3(0);

    //cam.defocus_angle = 10;
    //cam.focus_distance = length(point3(-0.5, 0, -0.2) - point3(0, 0, -0.2));


    // 2 Dragons

    cam.center = point3(-2, 0.8, -2.3);
    cam.lookat = point3(-0.85, 0.296, -0.225);

    cam.defocus_angle = 0;
    cam.focus_distance = length(cam.center - cam.lookat);
    
    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 200;
    rend.max_depth = 50;
    rend.background = color(0.2);
    
    timeFunction("render", [&] {
    rend.render(world, cam);
    });

    //rend.render(world, cam);

}

