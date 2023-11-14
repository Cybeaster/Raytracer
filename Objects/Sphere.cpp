#include "Sphere.h"
#include "../Ray/ORay.h"
#include "../Types/Hittable.h"
#include "../Utils/Color.h"

bool OSphere::Hit(const ORay&Ray, float TMin, float TMax, SHitRecord&OutHitRecord) const {
    using namespace Utils::Math;

    // Vector from sphere center to ray origin
    auto oc = Ray.GetOrigin() - Center;
    // Squared magnitude of the ray's direction
    auto a = LengthSquared(Ray.GetDirection());
    // Dot product of the vector from sphere center to ray origin and the ray's direction
    auto halfB = Dot(oc, Ray.GetDirection());
    // Squared magnitude of the vector from sphere center to ray origin
    auto c = LengthSquared(oc) - Squared(Radius);

    auto discriminant = Squared(halfB) - a * c;
    if (discriminant < 0) {
        return false;
    }

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    auto root = (-halfB - sqrtd) / a;
    if (root <= TMin || TMax <= root) {
        root = (-halfB + sqrtd) / a;
        if (root <= TMin || TMax <= root) {
            return false;
        }
    }

    // We have a valid intersection
    OutHitRecord.T = root;
    OutHitRecord.Point = Ray.PointAtParameter(OutHitRecord.T);
    const auto outwardNormal = (OutHitRecord.Point - Center) / Radius;
    OutHitRecord.SetFaceNormal(Ray, outwardNormal);

    return true;
}
