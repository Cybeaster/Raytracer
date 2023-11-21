#pragma once
#include "../Hittable.h"
#include "../../../Types/Math.h"

struct SHitRecord;
class SRay;

class OSphere : public IHittable
{
public:
	// Stationary sphere
	OSphere(SVec3 C, float R, std::shared_ptr<IMaterial> M);

	// Moving sphere
	OSphere(const SVec3& Point1, const SVec3& Point2, double R, std::shared_ptr<IMaterial> M);

	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const override;
	SVec3 GetPositionAtTime(const double Time) const;
	SAABB GetBoundingBox() const override;

private:
	SVec3 MoveVector = { 0, 0, 0 };
	bool IsMoving;
	SVec3 Center;
	float Radius;
	std::shared_ptr<IMaterial> Material;
	SAABB BoundingBox;
};


