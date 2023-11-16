#pragma once
#include "../../Types/Color.h"
#include "../SRay.h"

class SHitRecord;

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const = 0;
};
