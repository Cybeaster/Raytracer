#pragma once

#include "../Types/Math.h"
#include "../Ray/ORay.h"

namespace Utils {

    inline auto Lerp(float T, const SVec3 &A, const SVec3 &B) {
        return (1.0f - T) * A + T * B;
    }


    inline auto Normalize(const SVec3 &Vec) {
        return boost::qvm::normalized(Vec);
    }

    inline auto Dot(const SVec3 &A, const SVec3 &B) {
        return boost::qvm::dot(A, B);
    }

    inline auto SphereIntersection(const SVec3 &Center, float Radius, const ORay &Ray) {
        auto oc = Ray.GetOrigin() - Center;
        auto a = dot(Ray.GetDirection(), Ray.GetDirection());
        auto b = 2.0 * dot(oc, Ray.GetDirection());
        auto c = dot(oc, oc) - Radius * Radius;
        auto discriminant = b * b - 4 * a * c;
        return (discriminant >= 0);
    }
}


