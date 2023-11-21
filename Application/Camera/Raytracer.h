#pragma once
#include "..\..\Objects\Ray.h"
#include "../../Types/Math.h"
#include "../../Types/Color.h"

class IHittable;

class IRaytracer
{
public:
	virtual ~IRaytracer() = default;

	virtual SRay GetRay(float U, float V) const = 0;
	virtual SColor RayColor(const SRay& Ray, const IHittable& World) = 0;
};
