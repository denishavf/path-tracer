#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

template <typename T>
class vec3 {
    public:
        vec3() : e{0, 0, 0} {}
        vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}

        vec3 operator-() const {
            return vec3(-e[0], -e[1], -e[2]);
        }

        T operator[](int i) const {
            return e[i];
        }

        T& operator[](int i) {
            return e[i];
        }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const T &s) {
            e[0] *= s;
            e[1] *= s;
            e[2] *= s;
            return *this;
        }

        vec3& operator/=(const vec3 &s) {
            return *this *= 1/s;
        }

        T len_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        T len() const {
            return sqrt(len_squared());
        }

    private:
        T e[3];
};

template <typename T>
std::ostream& operator<<(std::ostream &out, const vec3<T> &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

template <typename T>
vec3<T> operator+(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u[0] + v[0], u[1] + v[1], u[2] + v[2]); 
}

template <typename T>
vec3<T> operator-(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u[0] - v[0], u[1] - v[1], u[2] - v[2]); 
}

template <typename T>
vec3<T> operator*(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u[0] * v[0], u[1] * v[1], u[2] * v[2]); 
}

template <typename T>
vec3<T> operator*(const T s, const vec3<T> &v) {
    return vec3<T>(s * v[0], s * v[1], s * v[2]); 
}

template <typename T>
vec3<T> operator*(const vec3<T> &v, const T s) { 
    return s * v;
}

template <typename T>
vec3<T> operator/(const vec3<T> &v, const T s) { 
    return (1/s) * v;
}

template <typename T>
T dot(const vec3<T> &u, const vec3<T> &v) {
    return u[0] * v[0] 
         + u[1] * v[1]
         + u[2] * v[2];
}

template <typename T>
vec3<T> cross(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(
                u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]
            );
}

template <typename T>
vec3<T> unit_vec(const vec3<T> &v) {
    return v / v.len(); 
}

template <typename T>
using point3 = vec3<T>;

template <typename T>
using color = vec3<T>;

