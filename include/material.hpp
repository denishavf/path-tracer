#pragma once

#include "utils.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "vec3.hpp"

//template <typename T>
//struct hit_record;

template <typename T>
class material {
    public:
        virtual bool scatter(
                const ray<T>& r_in,
                const hit_record<T>& rec,
                color<T>& attenuation,
                ray<T>& scattered
        ) const = 0;
};

template <typename T>
class lambertian : public material<T> {
    public: 
        lambertian(const color<T>& a) : albedo(a) {}

        virtual bool scatter (
                const ray<T>& r_in, 
                const hit_record<T>& rec, 
                color<T>& attenuation, 
                ray<T>& scattered) const override 
        {
            auto scattered_direction = rec.normal + random_unit_vector();

            if (scattered_direction.near_zero())
                scattered_direction = rec.normal;

            scattered = {rec.p, scattered_direction};
            attenuation = albedo;
            return true;
        }
    private:
        color<T> albedo;
};

template <typename T>
class metal : public material<T> {
    public: 
        metal(const color<T>& a, T f) 
            : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter (
                const ray<T>& r_in, 
                const hit_record<T>& rec, 
                color<T>& attenuation, 
                ray<T>& scattered) const override 
        {
            vec3<T> reflected = reflect(
                    unit_vec(r_in.direction()), rec.normal);
            scattered = {rec.p, reflected + fuzz * random_in_unit_sphere()};
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color<T> albedo;
        T fuzz;
};

template <typename T>
class dielectric : public material<T> {
    public: 
        dielectric(T index_of_refraction) 
            : ir(index_of_refraction){}

        virtual bool scatter (
                const ray<T>& r_in, 
                const hit_record<T>& rec, 
                color<T>& attenuation, 
                ray<T>& scattered) const override 
        {
            attenuation = {1.0, 1.0, 1.0};
            T refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            auto unit_direction = unit_vec(r_in.direction());
            T cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            T sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3<T> direction;
            
            if (
                    cannot_refract 
                    or reflectance(cos_theta, refraction_ratio) > random_double()) 
            {
                direction = reflect(unit_direction, rec.normal);
            }
            else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = {rec.p, direction};

            return true;
        }

    private:
        static T reflectance(double cosine, double ref_idx) {
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 *= r0;
            
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }

    private:
        T ir;
};
