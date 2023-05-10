#pragma once

#include "ray.hpp"
#include "utils.hpp"

template <typename T>
class material;

template <typename T>
struct hit_record {
    point3<T> p;
    vec3<T> normal;
    shared_ptr<material<T>> mat_ptr;
    T t;
    bool front_face;

    void set_face_normal(const ray<T>& r, const vec3<T>& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

template <typename T>
class hittable {
    public: 
        virtual bool hit(
                const ray<T>& r, 
                T t_min, 
                T t_max,
                hit_record<T>& rec
        ) const = 0;
};

