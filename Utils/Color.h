#pragma once
#include "../Types/Hittable.h"
#include "../Ray/ORay.h"
#include "../Types/Color.h"
#include "../Types/Math.h"
#include "Math.h"
#include <ostream>


class IHittable;

namespace Utils
{
inline void WriteColor(std::ostream& Out, const SColor& Color)
{
	Out << static_cast<int>(255.999f * Color.R) << ' '
		<< static_cast<int>(255.999f * Color.G) << ' '
		<< static_cast<int>(255.999f * Color.B) << '\n';
}

inline SColor GetColorAt(const ORay& Ray, const IHittable& World)
{
	using namespace Math;
	if (SHitRecord hitRecord; World.Hit(Ray, 0, Infinity, hitRecord))
	{
		return 0.5 * (hitRecord.Normal + SVec3{ 1, 1, 1 });
	}
	auto [a] = Normalize(Ray.GetDirection());
	auto res = 0.5f * (a[1] + 1.0f);
	return Lerp(res, SColor{ 1.0f, 1.0f, 1.0f }, SColor{ 0.5f, 0.7f, 1.0f });
}
} // namespace Utils
