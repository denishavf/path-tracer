#pragma once

#include "utils.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class camera {
    public:
        camera() {
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;

            origin = {0, 0, 0};
            horizontal = {viewport_width, 0.0, 0.0};
            vertical = {0.0, viewport_height, 0.0};
        }

        ray<double> get_ray(double u, double v) const {
            return {
                origin, 
                lower_left_corner + u*horizontal + v*vertical - origin
            };
        }

    private:
        point3<double> origin;
        point3<double> lower_left_corner;
        vec3<double> horizontal;
        vec3<double> vertical;
};
