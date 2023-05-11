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


hittable_list<double> random_scene() {
    hittable_list<double> world;

    auto ground_material = 
        make_shared<lambertian<double>>(color<double>(.5, .5, .5));  
    world.add(make_shared<sphere<double>>(point3<double>(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; ++a) {
        for (int b= -11; b < 11; ++b) {
            auto choose_mat = random_double();
            point3<double> center(
                    a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3<double>(4, 0.2, 0)).len() > 0.9) {
                shared_ptr<material<double>> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color<double>::random() * color<double>::random();
                    sphere_material = make_shared<lambertian<double>>(albedo);
                    world.add(make_shared<sphere<double>>(
                                center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    //metal
                    auto albedo = color<double>::random(.5, 1);
                    auto fuzz = random_double(.5, 1);
                    sphere_material = make_shared<metal<double>>(albedo, fuzz);
                    world.add(make_shared<sphere<double>>(
                                center, .2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric<double>>(0.5);
                    world.add(make_shared<sphere<double>>(
                                center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric<double>>(1.5);
    world.add(make_shared<sphere<double>>(point3<double>(0, 1, 0), 1.0, material1));

    return world;
}

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
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    // world
    auto world = random_scene();

    // camera
    point3<double> lookfrom(13, 2, 3); 
    point3<double> lookat(0, 0, 0);
    vec3<double> vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1; 

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

