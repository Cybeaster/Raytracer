#pragma once
#include "Material.h"
#include "../Hittable/Hittable.h"
#include "../Textures/Texture.h"


class OIsotropic : public IMaterial
{
public:
	OIsotropic(shared_ptr<ITexture> _Albedo)
		: Albedo(_Albedo)
	{
	}

	OIsotropic(const SColor& Color)
		: Albedo(make_shared<OSolidColor>(Color))
	{
	}

	bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const override
	{
		//Pick random direction
		OutScattered = SRay(HitRecord.Point, Utils::Math::RandomUnitVector(), Ray.GetTime());
		OutAttenuation = Albedo->Value(HitRecord.U, HitRecord.V, HitRecord.Point);
		return true;
	}

private:
	shared_ptr<ITexture> Albedo;
};
