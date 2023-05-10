#pragma once

#include "utils.hpp"
#include "ray.hpp"
#include "hittable.hpp"

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
        metal(const color<T>& a) : albedo(a) {}

        virtual bool scatter (
                const ray<T>& r_in, 
                const hit_record<T>& rec, 
                color<T>& attenuation, 
                ray<T>& scattered) const override 
        {
            vec3<T> reflected = reflect(
                    unit_vec(r_in.direction()), rec.normal);
            scattered = {rec.p, reflected};
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color<T> albedo;
};
