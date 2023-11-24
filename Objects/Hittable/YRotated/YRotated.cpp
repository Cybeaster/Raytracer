#include "YRotated.h"


ORotatedY::ORotatedY(shared_ptr<IHittable> Object, const double Angle)
	: RotatedObject(Object)
{
	using namespace Utils::Math;
	auto radians = DegreesToRaians(Angle);
	SinTheta = sin(radians);
	CosTheta = cos(radians);

	BoundingBox = Object->GetBoundingBox();
	SVec3 Min{ INFINITY, INFINITY, INFINITY };
	SVec3 Max{ -INFINITY, -INFINITY, -INFINITY };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				// Calculate coordinates of the corner
				float x = i * BoundingBox.X.Max + (1 - i) * BoundingBox.X.Min;
				float y = j * BoundingBox.Y.Max + (1 - j) * BoundingBox.Y.Min;
				float z = k * BoundingBox.Z.Max + (1 - k) * BoundingBox.Z.Min;

				// Rotate the corner around the Y-axis
				float newX = CosTheta * x + SinTheta * z;
				float newZ = -SinTheta * x + CosTheta * z;
				SVec3 tester{ newX, y, newZ };

				// Update the min and max coordinates of the new bounding box
				for (int c = 0; c < 3; c++)
				{
					Min.a[c] = fmin(Min.a[c], tester.a[c]);
					Max.a[c] = fmax(Max.a[c], tester.a[c]);
				}
			}
		}
	}
	BoundingBox = SAABB(Min, Max);
}

bool ORotatedY::Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const
{
	// Rotate the ray origin and direction to 'undo' the object rotation
	auto origin = Ray.GetOrigin();
	auto direction = Ray.GetDirection();

	origin.a[0] = CosTheta * Ray.GetOrigin().a[0] - SinTheta * Ray.GetOrigin().a[2];
	origin.a[2] = SinTheta * Ray.GetOrigin().a[0] + CosTheta * Ray.GetOrigin().a[2];

	direction.a[0] = CosTheta * Ray.GetDirection().a[0] - SinTheta * Ray.GetDirection().a[2];
	direction.a[2] = SinTheta * Ray.GetDirection().a[0] + CosTheta * Ray.GetDirection().a[2];

	// Create a new ray with the rotated origin and direction
	SRay rotated(origin, direction, Ray.GetTime());

	// Perform intersection test with the rotated ray
	if (!RotatedObject->Hit(rotated, Interval, HitRecord))
	{
		return false;
	}

	// Adjust the hit point and normal to account for the rotation
	auto p = HitRecord.Point;
	p.a[0] = CosTheta * HitRecord.Point.a[0] + SinTheta * HitRecord.Point.a[2];
	p.a[2] = -SinTheta * HitRecord.Point.a[0] + CosTheta * HitRecord.Point.a[2];

	auto normal = HitRecord.Normal;
	normal.a[0] = CosTheta * HitRecord.Normal.a[0] + SinTheta * HitRecord.Normal.a[2];
	normal.a[2] = -SinTheta * HitRecord.Normal.a[0] + CosTheta * HitRecord.Normal.a[2];

	// Update the hit record with the adjusted hit point and normal
	HitRecord.Point = p;
	HitRecord.Normal = normal;
	return true;
}
