#include "../include/utils.hpp"
#include "../include/vec3.hpp"
#include "../include/ray.hpp"
#include "../include/color.hpp"
#include "../include/camera.hpp"

#include "../include/hittable-list.hpp"
#include "../include/sphere.hpp"

#include <iostream>

color<double> ray_color(
        const ray<double>& r, 
        const hittable<double>& world)
{
    hit_record<double> rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color<double>(1, 1, 1));
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

    // world
    hittable_list<double> world;
    world.add(make_shared<sphere<double>>(point3<double>(0, 0, -1), 0.5));
    world.add(make_shared<sphere<double>>(point3<double>(0, -100.5, -1), 100));

    // camera
    camera cam;

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
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nD\n";

    return 0;
}

