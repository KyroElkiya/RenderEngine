#ifndef RENDERER_H
#define RENDERER_H

#include "rayHitInfo.h"
#include "components/materials.h"
#include "vec3.h"
#include "components/camera.h"

class renderer {
public:
    double aspect_ratio      = 1.0; // Image width to height ratio.
    int    image_width       = 100; // Rendered image width in pixels.
    int    samples_per_pixel = 10;  // Total number of random samples used per pixel.
    int    max_depth         = 10;  // Max number of ray bounces in scene.

    void render(const sceneObject &world, camera &cam) {
        initialize(cam);

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
            
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0.0, 0.0, 0.0);

                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_sample_scale * pixel_color);
            }
        }
        std::clog << "\rDone.          \n";
    }

private:
    int    image_height;       // Rendered image height
    point3 pixel00_loc;        // Location of pixel 0, 0
    double pixel_sample_scale; // Color scale factor for a sum of pixel samples 
    vec3   pixel_delta_u;      // Offset to pixel to the right 
    vec3   pixel_delta_v;      // Offset to pixel below
    vec3   camera_center;      // Position of camera

    void initialize(camera cam) {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1: image_height;

        pixel_sample_scale = 1.0 / samples_per_pixel;

        camera_center = cam.center;
        auto focal_length = cam.focal_length;

        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        auto viewport_u = vec3(viewport_width, 0.0, 0.0);
        auto viewport_v = vec3(0.0, -viewport_height, 0.0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_top_left_pixel = camera_center - vec3(0.0, 0.0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_top_left_pixel + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_squared();
        auto pixel_sample = pixel00_loc + ((i + offset.x) * pixel_delta_u) + ((j + offset.y) * pixel_delta_v);
        auto ray_origin = camera_center;
        auto ray_dir = pixel_sample - ray_origin;

        return ray(ray_origin, ray_dir);
    }

    vec3 sample_squared() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
    }

    color ray_color(ray &r, int depth, const sceneObject &world) {
        if (depth <= 0) 
            return color(0.0, 0.0, 0.0);

        rayHitInfo ray_hit_info;
        if(world.intersect(r, interval(1e-8, infinity), ray_hit_info)) {
            ray scattered;
            color attenuation;

            if (ray_hit_info.mat->scatter(r, ray_hit_info, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0.0, 0.0, 0.0);
        }
    vec3 n_dir = normalize(r.direction());
    double a = 0.5 * (n_dir.y + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
    
};


#endif
