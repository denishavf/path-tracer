#include "../include/utils.hpp"
#include "../include/vec3.hpp"
#include "../include/ray.hpp"
#include "../include/color.hpp"
#include "../include/camera.hpp"

#include "../include/hittable-list.hpp"
#include "../include/sphere.hpp"

#include "../include/material.hpp"

#include <iostream>
#include <memory>

color<double> ray_color(
        const ray<double>& r, 
        const hittable<double>& world,
        int depth)
{
    if (depth <= 0) {
        return {0, 0, 0};
    }

    hit_record<double> rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray<double> scattered;
        color<double> attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color<double>(0, 0, 0);
        //point3<double> target = rec.p + rec.normal + random_unit_vector();
        //return 0.5 * ray_color(ray<double>(rec.p, target - rec.p), world, depth-1);
    }

    auto unit_direction = unit_vec<double>(r.direction());
    auto t = 0.5 * (unit_direction[1] + 1.0);
    return (1.0 - t)  * color<double>(1, 1, 1) 
            + t * color<double>(0.5, 0.7, 1.0);
}

int main() {
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // world
    auto R = cos(pi / 4);
    hittable_list<double> world;

    
    auto material_ground = 
        make_shared<lambertian<double>>(color<double>(.8, .8, .0));
    auto material_center = 
        make_shared<lambertian<double>>(color<double>(0.1, 0.2, 0.5));
    auto material_left = 
        make_shared<dielectric<double>>(1.5);
    auto material_right = 
        make_shared<metal<double>>(color<double>(.8, .6, .2), 0.0);

    world.add(make_shared<sphere<double>>(
                point3<double>(.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere<double>>(
                point3<double>(.0, .0, -1.0), .5, material_center));
    world.add(make_shared<sphere<double>>(
                point3<double>(-1.0, .0, -1.0), .5, material_left));
    world.add(make_shared<sphere<double>>(
                point3<double>(-1.0, .0, -1.0), -0.4, material_left));
    world.add(make_shared<sphere<double>>(
                point3<double>(1.0, .0, -1.0), .5, material_right));

    // camera
    point3<double> lookfrom(3, 3, 2); 
    point3<double> lookat(0, 0, -1);
    vec3<double> vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).len();
    auto aperture = 2.0; 

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // render
    std::cout 
              <<  "P3\n" 
              << image_width << ' ' << image_height 
              << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i != image_width; ++i) {
            color<double> pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nD\n";

    return 0;
}

