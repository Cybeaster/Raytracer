#pragma once
#include "../Hittable.h"


class OQuad : public IHittable
{
public:
	OQuad(const SVec3& _Q, const SVec3& _U, const SVec3& _V, const shared_ptr<IMaterial>& _M);

	virtual void SetBoundingBox();
	SAABB GetBoundingBox() const override;
	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const override;
	virtual bool IsInterior(double A, double B, SHitRecord& HitRecord) const;

private:
	SVec3 Origin;
	SVec3 U;
	SVec3 V;
	SVec3 W;
	shared_ptr<IMaterial> Material;
	SAABB BoundingBox;
	SVec3 Normal;
	double Offset;
};
