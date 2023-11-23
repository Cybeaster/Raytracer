#pragma once
#include "../../Types/Types.h"
#include "Material.h"
#include "../Textures/Texture.h"

class ODiffuseLight : public IMaterial
{
public:
	ODiffuseLight(const shared_ptr<ITexture>& Light)
		: Emit(Light)
	{
	}

	ODiffuseLight(const SColor& Color)
		: Emit(make_shared<OSolidColor>(Color))
	{
	}

	virtual bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& Attenuation, SRay& Scattered) const override
	{
		return false;
	}

	SColor Emitted(double U, double V, const SVec3& Point) const override
	{
		return Emit->Value(U, V, Point);
	}

private:
	shared_ptr<ITexture> Emit;
};
