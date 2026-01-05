#pragma once

#include "vec3.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the range [0,255]
    // scaling by 255.999 is a useful mathematical trick to skip
    // extra rounding instructions, fp error typically is 1e-6, 1e-7
    // this ensures numbers very close to 1.0 get treated the same way
    // applies to other normalized pixel color values
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    std::cout << rbyte << " " << gbyte << " " << bbyte << std::endl;
}
