#pragma once

#include "../Types/Math.h"
#include "../Ray/ORay.h"
#include <ostream>

namespace Utils {

    void WriteColor(std::ostream &Out, const SColor &Color) {
        Out << static_cast<int>(255.999f * Color.a[0]) << ' '
            << static_cast<int>(255.999f * Color.a[1]) << ' '
            << static_cast<int>(255.999f * Color.a[2]) << '\n';
    }

    SColor GetColorAt(const ORay &Ray) {
        SVec3 unitDir = Normalize(Ray.GetDirection());
        auto t = 0.5f * (unitDir.a[1] + 1.0f);
        return Lerp(t, SColor{1.0f, 1.0f, 1.0f}, SColor{0.5f, 0.7f, 1.0f});
    }
} // namespace Utils
