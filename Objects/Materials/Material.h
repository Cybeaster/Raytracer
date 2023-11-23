#pragma once
#include "../../Types/Color.h"
#include "..\Ray.h"
#include "../../Types/Types.h"
class SHitRecord;

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const = 0;
	virtual SColor Emitted(double U, double V, const SVec3& Point) const { return SColor(0, 0, 0); }


	template<typename T, typename... Args>
	static shared_ptr<IMaterial> CreateMaterial(Args&&... args)
	{
		return make_shared<T>(forward<Args>(args)...);
	}
};
