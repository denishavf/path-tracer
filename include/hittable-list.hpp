#pragma once

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

template <typename T>
class hittable_list : public hittable<T> {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable<T>> object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(shared_ptr<hittable<T>> object) {
            objects.push_back(object);
        }

        virtual bool hit(
                const ray<T>& r, 
                T t_min, 
                T t_max,
                hit_record<T>& rec
        ) const override;

    private:
        std::vector<shared_ptr<hittable<T>>> objects;
};

template <typename T>
bool hittable_list<T>::hit(
        const ray<T>& r, 
        T t_min, 
        T t_max,
        hit_record<T>& rec) const 
{
    hit_record<T> temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
