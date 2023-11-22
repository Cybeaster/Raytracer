#pragma once
#include "../../Types/Interval/Interval.h"
#include "../../Types/Math.h"
#include "../../Types/AABB/AABB.h"
#include "../../Utils/Math.h"
#include "../../Utils/Exceptions.h"

#include <memory>


class SRay;
class IMaterial;

struct SHitRecord
{
	void SetFaceNormal(const SRay& Ray, const SVec3& OutwardNormal)
	{
		FrontFace = Utils::Math::Dot(Ray.GetDirection(), OutwardNormal) < 0;
		Normal = FrontFace ? OutwardNormal : -OutwardNormal;
	}


	SVec3 Point;
	SVec3 Normal;
	double T;

	// Texture coordinates
	double U;
	double V;

	bool FrontFace;
	std::shared_ptr<IMaterial> Material;
};

class IHittable
{
public:
	virtual ~IHittable() = default;
	virtual bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const = 0;
	virtual SAABB GetBoundingBox() const = 0;
};
