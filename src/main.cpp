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

void two_dragons_lambertian() {
    
    sceneObjects world;
    
    auto material_lambert_orange = make_shared<lambertian>(color(0.9, 0.2, 0.05));
    auto material_lambert_green  = make_shared<lambertian>(color(0.1, 0.7, 0.2));
    auto material_ground         = make_shared<lambertian>(color(0.02, 0.5, 0.8));

    print("Creating Lights");
    auto material_emit = make_shared<emitter>(color(0.98, 0.98, 0.98), 50);
    world.add(make_shared<sphere>(point3(0, 2.5, 0), 0.5, material_emit));

    print("Creating Shared Pointers");
    shared_ptr<mesh> fg_dragon    = std::make_shared<mesh>(material_lambert_orange);
    shared_ptr<mesh> bg_dragon    = std::make_shared<mesh>(material_lambert_green);
    shared_ptr<mesh> dragon_floor = std::make_shared<mesh>(material_ground);
    
    print("Loading Meshes");
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/two_dragons/dragon_high_fg.obj", fg_dragon);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/two_dragons/dragon_high_bg.obj", bg_dragon);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/two_dragons/floor_two_dragons.obj", dragon_floor);
    
    print("Finalizing Meshes");
    fg_dragon->finalize();
    bg_dragon->finalize();
    dragon_floor->finalize();

    print("Adding Meshes to World");
    world.add(fg_dragon);
    world.add(bg_dragon);
    world.add(dragon_floor);

    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    cam.vfov = 30;
    cam.center = point3(-2, 0.8, -2.3);
    cam.lookat = point3(-0.85, 0.296, -0.225);
    cam.defocus_angle = 0;
    cam.focus_distance = length(cam.center - cam.lookat);
    
    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 400;
    rend.max_depth = 50;
    rend.background = color(0.2);

    timeFunction("render", [&] {
    rend.render(world, cam);
    });
}

void two_dragons_metallic() {
    
    sceneObjects world;
    
    auto material_rose_gold = make_shared<metal>(color(0.64, 0.34, 0.31), 0);
    auto material_chrome    = make_shared<metal>(color(0.98), 0);
    auto material_ground    = make_shared<lambertian>(color(0.02, 0.5, 0.8));

    print("Creating Lights");
    auto material_emit = make_shared<emitter>(color(0.98, 0.98, 0.98), 50);
    world.add(make_shared<sphere>(point3(0, 2.5, 0), 0.5, material_emit));

    print("Creating Shared Pointers");
    shared_ptr<mesh> fg_dragon    = std::make_shared<mesh>(material_rose_gold);
    shared_ptr<mesh> bg_dragon    = std::make_shared<mesh>(material_chrome);
    shared_ptr<mesh> dragon_floor = std::make_shared<mesh>(material_ground);
    
    print("Loading Meshes");
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/two_dragons/dragon_high_fg.obj", fg_dragon);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/two_dragons/dragon_high_bg.obj", bg_dragon);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/two_dragons/floor_two_dragons.obj", dragon_floor);
    
    print("Finalizing Meshes");
    fg_dragon->finalize();
    bg_dragon->finalize();
    dragon_floor->finalize();

    print("Adding Meshes to World");
    world.add(fg_dragon);
    world.add(bg_dragon);
    world.add(dragon_floor);

    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    cam.vfov = 30;
    cam.center = point3(-2, 0.8, -2.3);
    cam.lookat = point3(-0.85, 0.296, -0.225);
    cam.defocus_angle = 0;
    cam.focus_distance = length(cam.center - cam.lookat);
    
    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 400;
    rend.max_depth = 50;
    rend.background = color(0);
    
    timeFunction("render", [&] {
    rend.render(world, cam);
    });
}

void platonic_solids() {
    
    sceneObjects world;
    
    auto material_lambert_blue  = make_shared<lambertian>(color(0.0, 0.5, 0.8));
    auto material_lambert_green = make_shared<lambertian>(color(0.1, 0.7, 0.2));
    auto material_glass         = make_shared<dieletric>(1.5);
    auto material_yellow_metal  = make_shared<metal>(color(0.9, 0.7, 0.0), 0.0);
 
    print("Creating Lights");
    auto material_emit = make_shared<emitter>(color(0.98, 0.98, 0.98), 50);
    //world.add(make_shared<sphere>(point3(0, 2.5, 0), 0.5, material_emit));

    print("Creating Shared Pointers");
    shared_ptr<mesh> center_platonic = std::make_shared<mesh>(material_lambert_green);
    shared_ptr<mesh> right_platonic  = std::make_shared<mesh>(material_glass);
    shared_ptr<mesh> left_platonic   = std::make_shared<mesh>(material_yellow_metal);
    shared_ptr<mesh> groundplane     = std::make_shared<mesh>(material_lambert_blue);

    print("Loading Meshes");
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/platonic_solids/center_platonic.obj", center_platonic);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/platonic_solids/right_platonic.obj", right_platonic);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/platonic_solids/left_platonic.obj", left_platonic);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/platonic_solids/groundplane.obj", groundplane);
    
    print("Finalizing Meshes");
    center_platonic->finalize();
    right_platonic->finalize();
    left_platonic->finalize();
    groundplane->finalize();

    print("Adding Meshes to World");
    world.add(center_platonic);
    world.add(right_platonic);
    world.add(left_platonic);
    world.add(groundplane);
    
    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    cam.vfov = 90;
    cam.center = point3(0);

    //cam.defocus_angle = 10;
    //cam.focus_distance = length(point3(0) - point3(0, 0, -1));
    
    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 10;
    rend.max_depth = 50;
    //rend.background = color(0);
    
    timeFunction("render", [&] {
    rend.render(world, cam);
    });
}

void cornell_dragon() {

    sceneObjects world;

    auto material_red   = make_shared<lambertian>(color(0.9, 0.2, 0.2));
    auto material_blue  = make_shared<lambertian>(color(0.2, 0.2, 0.9));
    auto material_white = make_shared<lambertian>(color(0.8, 0.8, 0.8));
    auto material_glass = make_shared<dieletric>(1.5);
    auto material_light = make_shared<emitter>(color(0.8), 5);

    print("Creating Shared Pointers");
    shared_ptr<mesh> cornell_dragon = std::make_shared<mesh>(material_glass);
    shared_ptr<mesh> cornell_red = std::make_shared<mesh>(material_red);
    shared_ptr<mesh> cornell_blue = std::make_shared<mesh>(material_blue);
    shared_ptr<mesh> cornell_white = std::make_shared<mesh>(material_white);
    shared_ptr<mesh> cornell_light = std::make_shared<mesh>(material_light);
    shared_ptr<mesh> cornell_backface = std::make_shared<mesh>(material_white);
    
    print("Loading Meshes");
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/cornell_dragon/dragon_cornell.obj", cornell_dragon);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/cornell_dragon/cornell_red.obj", cornell_red);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/cornell_dragon/cornell_blue.obj", cornell_blue);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/cornell_dragon/cornell_white.obj", cornell_white);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/cornell_dragon/cornell_light.obj", cornell_light);
    objLoader::load("/home/Andrew/Documents/University/Showreel/RenderEngine/Scenes/cornell_dragon/cornell_backface.obj", cornell_backface);

    print("Finalizing Meshes");
    cornell_dragon->finalize();
    cornell_red->finalize();
    cornell_blue->finalize();
    cornell_white->finalize();
    cornell_light->finalize();
    cornell_backface->finalize();

    print("Adding Meshes to World");
    world.add(cornell_dragon);
    world.add(cornell_red);
    world.add(cornell_blue);
    world.add(cornell_white);
    //world.add(cornell_backface);
    world.add(cornell_light);

    world = sceneObjects(make_shared<bvh_node>(world));

    camera cam;
    cam.vfov = 30;
    cam.center = point3(0, 0, -2);

    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 400;
    rend.max_depth = 50;
    rend.background = color(0);
    
    timeFunction("render", [&] {
    rend.render(world, cam);
    });
}

void nessy() {
    
    sceneObjects world;

    auto material_matte = make_shared<lambertian>(color(.2, .4, .9));
    shared_ptr<mesh>nessy = make_shared<mesh>(material_matte);
    objLoader::load("/home/Andrew/Downloads/nessy.obj", nessy);
    nessy->finalize();
    world.add(nessy);

    auto material_blue = make_shared<emitter>(color(0.7, 0.7, 0.9), 30);
    auto material_red  = make_shared<emitter>(color(0.9, 0.2, 0.2), 40);
    world.add(make_shared<sphere>(point3(1, 1, 2.5), 0.3, material_blue));
    world.add(make_shared<sphere>(point3(0, 5, -3), 1, material_red));

    camera cam;
    cam.vfov = 30;
    cam.center = point3(-1, 1, 4);
    cam.lookat = point3(0, 0.5, 0);
    
    renderer rend;

    rend.aspect_ratio = 16.0/9.0;
    rend.image_width = 1920;
    rend.samples_per_pixel = 400;
    rend.max_depth = 50;
    rend.background = color(0);
    
    timeFunction("render", [&] {
    rend.render(world, cam);
    });
}

int main() {
    switch(4) {
        case 0: platonic_solids();  break;
        case 1: two_dragons_metallic(); break;
        case 2: two_dragons_lambertian(); break;
        case 3: cornell_dragon(); break;
        case 4: nessy(); break;
    }
}

