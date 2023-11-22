#pragma once
#include "Material.h"
#include "../../Types/Color.h"
#include "../Hittable/Hittable.h"
#include "..\Ray.h"
#include "../Textures/Texture.h"

class OLambertian : public IMaterial
{
public:
	OLambertian(const SColor& A)
		: Albedo(make_shared<OSolidColor>(A))
	{
	}

	OLambertian(const shared_ptr<ITexture>& A)
		: Albedo(A)
	{
	}

	bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const override
	{
		auto scatterDirection = HitRecord.Normal + Utils::Math::RandomUnitVector();

		if (Utils::Math::IsNearZero(scatterDirection))
		{
			scatterDirection = HitRecord.Normal;
		}

		OutScattered = SRay(HitRecord.Point, scatterDirection, Ray.GetTime());
		OutAttenuation = Albedo->Value(HitRecord.U, HitRecord.V, HitRecord.Point);
		return true;
	}

private:
	shared_ptr<ITexture> Albedo;
};
