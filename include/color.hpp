#pragma once

#include "vec3.hpp"

#include <iostream>

template <typename T>
void write_color(std::ostream &out, color<T> pixel_color) {
    out << static_cast<int>(255.999 * pixel_color[0]) << ' '
        << static_cast<int>(255.999 * pixel_color[1]) << ' '
        << static_cast<int>(255.999 * pixel_color[2]) << '\n';
}

