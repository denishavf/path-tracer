#pragma once

#include "hittable.hpp"
#include "vec3.hpp"
#include <memory>

template <typename T>
class sphere : public hittable<T> {
    public: 
        sphere() {}
        sphere(point3<T>cen, T r, shared_ptr<material<T>> m) 
            : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
                const ray<T>& r,
                T t_min,
                T t_max,
                hit_record<T>& rec
        ) const override;

    private:
        point3<T> center;
        T radius;
        shared_ptr<material<T>> mat_ptr;
};

template <typename T>
bool sphere<T>::hit(
            const ray<T>& r,
            T t_min,
            T t_max,
            hit_record<T>& rec) const 
{
    vec3<T> oc = r.origin() - center;  
    auto a = r.direction().len_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.len_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) 
        return false;
    auto sqrtd = sqrt(discriminant);
    
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min or t_max < root) {
        root = (-half_b + sqrtd) / a;

        if (root < t_min or t_max < root) {
            return false;
        }
    }

    rec.t = root;
    //#rec.p - r.at(rec.t);
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, (rec.p - center) / radius);
    rec.mat_ptr = mat_ptr;

    return true;
}
