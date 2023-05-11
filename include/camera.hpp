#pragma once

#include "utils.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class camera {
    public:
        camera(
                point3<double> lookfrom,
                point3<double> lookat,
                vec3<double> vup,
                double vfov, 
                double aspect_ratio,
                double aperture,
                double focus_dist) 
        {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vec(lookfrom - lookat);
            u = unit_vec(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;

            lower_left_corner = 
                  origin 
                - horizontal / 2. 
                - vertical / 2. 
                - focus_dist*w;

            lens_radius = aperture / 2;
        }

        ray<double> get_ray(double u, double v) const {
            auto rd = lens_radius * random_in_unit_disk();
            auto offset = u * rd[0] + v * rd[1];
            return {
                origin + offset, 
                lower_left_corner + u*horizontal + v*vertical - origin - offset
            };
        }

    private:
        point3<double> origin;
        point3<double> lower_left_corner;
        vec3<double> horizontal;
        vec3<double> vertical;
        vec3<double> u, v, w;
        double lens_radius;
};
