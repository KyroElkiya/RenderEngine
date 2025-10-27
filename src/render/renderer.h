#pragma once

#include "../core/interaction/rayHitInfo.h"
#include "../core/math/vec3.h"
#include "../camera/camera.h"
#include "../core/threadpool.h"
#include "../core/utils.h"
#include "../material/material.h"
#include "../core/color.h"
#include "../io/ppm_exporter.cpp"

#include <omp.h>
#include <thread>
#include <mutex>
#include <atomic>

class renderer {
public:
    double aspect_ratio      = 1.0; // Image width to height ratio.
    int    image_width       = 100; // Rendered image width in pixels.
    int    samples_per_pixel = 10;  // Total number of random samples used per pixel.
    int    max_depth         = 10;  // Max number of ray bounces in scene.
    color  background        = color(0.5, 0.7, 1);

    void render(const sceneObject &world, camera &cam) {
        initialize(cam);

        //std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        std::vector<std::vector<color>> framebuffer(image_height, std::vector<color>(image_width));

        threadPool pool(std::thread::hardware_concurrency());

        const int chunk_size = 8;
        std::atomic<int> rows_completed = 0;
        std::mutex clog_mutex; // Logging

        
        for (int start_row = 0; start_row < image_height; start_row += chunk_size) {
            int end_row = std::min(start_row + chunk_size, image_height);

            pool.queue([&, start_row, end_row]() {
                for (int j = start_row; j < end_row; j++) {
                    for (int i = 0; i < image_width; i++) {

                    
                        color pixel_color(0);
                        for (int sample = 0; sample < samples_per_pixel; sample++) {
                            ray r = get_ray(i, j);
                            pixel_color += new_ray_color(r, max_depth, world);
                            if (!std::isfinite(pixel_color.x) || !std::isfinite(pixel_color.y) || !std::isfinite(pixel_color.z))
                                std::cout << "Pixel Color NaN: " << i << " " << j << " " << pixel_color.x << " " << pixel_color.y << " " << pixel_color.z << std::endl;
                            //pixel_color = normals_ray_color(r, world);
                        }
                        framebuffer[j][i] = pixel_sample_scale * pixel_color;
                    }

                    int completed = rows_completed++;
                    {
                        std::lock_guard<std::mutex> lock(clog_mutex);
                        std::clog << "\rScanlines remaining: " << (image_height - completed) << " " << std::flush;
                    }
                }
            
            });
        }
        pool.wait();
        
        write_ppm("/home/Andrew/Documents/University/Showreel/RenderEngine/Engine/output/test.ppm", image_width, image_height, framebuffer);

        std::clog << "\rDone.                                          \n";
    }

private:
    int    image_height;       // Rendered image height
    point3 pixel00_loc;        // Location of pixel 0, 0
    double pixel_sample_scale; // Color scale factor for a sum of pixel samples 
    vec3   pixel_delta_u;      // Offset to pixel to the right 
    vec3   pixel_delta_v;      // Offset to pixel below
    vec3   camera_center;      // Position of camera
    vec3   u, v, w;            // Camera frame basis vectors
    vec3   defocus_disk_u;     // Defocus disk horizontal radius
    vec3   defocus_disk_v;     // Defocus disk vertical radius
    double defocus_angle;


    void initialize(camera cam) {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1: image_height;

        pixel_sample_scale = 1.0 / samples_per_pixel;

        camera_center = cam.center;
        defocus_angle = cam.defocus_angle;
        auto theta = degrees_to_radians(cam.vfov);
        auto h = std::tan(theta/2);
    
        w = normalize(camera_center - cam.lookat);
        u = normalize(cross(cam.vup, w));
        v = cross(w, u);

        auto viewport_height = 2.0 * h * cam.focus_distance;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        auto viewport_u = viewport_width  *  u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_top_left_pixel = camera_center - (cam.focus_distance * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_top_left_pixel + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = cam.focus_distance * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;

    }

    ray get_ray(int i, int j) const {
        //Construct camera ray originated from defocus disk and directed at a random sample point around location i, j

        auto offset = sample_squared();
        auto pixel_sample = pixel00_loc + ((i + offset.x) * pixel_delta_u) + ((j + offset.y) * pixel_delta_v);
        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_dir = normalize(pixel_sample - ray_origin);

        if (length2(ray_dir) < 1e-8) ray_dir = vec3(0, 0, -1);

        return ray(ray_origin, ray_dir);
    }

    vec3 sample_squared() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
    }

    vec3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return camera_center + (p.x *defocus_disk_u) + (p.y * defocus_disk_v);
    }

    color normals_ray_color(ray &r, const sceneObject &world) {
        rayHitInfo ray_hit_info;
        if(world.intersect(r, interval(EPSILON, infinity), ray_hit_info)) {
            return ray_hit_info.front_face ? color(1, 0, 0) : color(0, 0, 1);
        }
        
        vec3 n_dir = normalize(r.direction());
        double a = 0.5 * (n_dir.y + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    color ray_color(ray &r, int depth, const sceneObject &world) {
        if (depth <= 0) 
            return color(0, 0, 0);

        rayHitInfo ray_hit_info;
        if(world.intersect(r, interval(0.001, infinity), ray_hit_info)) {
            ray scattered;
            color attenuation;

            if (ray_hit_info.mat->scatter(r, ray_hit_info, attenuation, scattered)) {
                //nan_vec3(ray_hit_info.N, "Ray Hit Normal");
                //nan_vec3(scattered.direction(), "Scattered Direction");
                return attenuation * ray_color(scattered, depth-1, world); 
            }
            return color(0.0, 0.0, 0.0);
        }
    vec3 n_dir = normalize(r.direction());
    double a = 0.5 * (n_dir.y + 1.0);
    //return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    //return (1.0 - a) * color(1) + a * color(0, 0, 0.5);
    return color(0);
    }
    
    color new_ray_color(ray &r, int depth, const sceneObject &world) {
        if (depth <= 0)
            return color(0);

        rayHitInfo ray_hit_info;

        if (!world.intersect(r, interval(0.001, infinity), ray_hit_info)) {
            return background;
        }
        
        ray scattered;
        color attenuation;
        vec3 emitted = ray_hit_info.mat->emit();

        if (!ray_hit_info.mat->scatter(r, ray_hit_info, attenuation, scattered))
            return emitted;

        return emitted + (attenuation * new_ray_color(scattered, depth-1, world));

    }


};


