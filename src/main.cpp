#include "../include/color.hpp"
#include "../include/vec3.hpp"
#include "../include/ray.hpp"

#include <iostream>

bool hit_sphere(const point3<double>& center, double radius, const ray<double>& r) {
    auto oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

color<double> ray_color(const ray<double>& r) {
    if (hit_sphere(point3<double>(0, 0, -1) , 0.5, r)) {
        return color<double>(1, 0, 0);
    }

    vec3<double> unit_direction = unit_vec(r.direction());
    auto t = 0.5 * (unit_direction[1] + 1.0);
    return (1.0 - t) * color<double>(1, 1, 1) + t * color<double>(.5, .7, 1);
}

int main() {
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    point3<double> origin(0, 0, 0);
    vec3<double> horizontal(viewport_width, 0, 0);
    vec3<double> vertical(0, viewport_height, 0);
    auto lower_left_corner = 
                            origin 
                            - horizontal/2.0 
                            - vertical/2.0 
                            - vec3<double>(0, 0, focal_length); 

    // render
    std::cout 
              <<  "P3\n" 
              << image_width << ' ' << image_height 
              << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i != image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray<double> r(
                    origin, 
                    lower_left_corner + u*horizontal + v*vertical - origin
            );
            color<double> pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nD\n";

    return 0;
}

