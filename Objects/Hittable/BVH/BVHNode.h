#pragma once
#include "../Hittable.h"
#include "../../../Types/AABB/AABB.h"
#include "../List/HittableList.h"
#include "../../../Types/Interval/Interval.h"

/**
 * \brief A node in a Spatial Bounding Volume Hierarchy.
 * \details Used to detect collisions between objects in a scene.
 * \link <https://www.nvidia.in/docs/IO/77714/sbvh.pdf>
 */
class SBVHNode : public IHittable
{
public:
	SBVHNode(const OHittableList& List);

	/*
	 * Builds a BVH from a list of objects with a given start and end index.
	 */
	SBVHNode(const std::vector<std::shared_ptr<IHittable>>& Objects, size_t Start, size_t End);

	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const override;

	SAABB GetBoundingBox() const override
	{
		return BoundingBox;
	}

private:
	inline static bool BoxCompare(const std::shared_ptr<IHittable> A, const std::shared_ptr<IHittable> B, uint8_t AxisIndex)
	{
		return A->GetBoundingBox().Axis(AxisIndex).Min < B->GetBoundingBox().Axis(AxisIndex).Min;
	}

	inline static bool BoxXCompare(const std::shared_ptr<IHittable> A, const std::shared_ptr<IHittable> B)
	{
		return BoxCompare(A, B, 0);
	}

	inline static bool BoxYCompare(const std::shared_ptr<IHittable> A, const std::shared_ptr<IHittable> B)
	{
		return BoxCompare(A, B, 1);
	}

	inline static bool BoxZCompare(const std::shared_ptr<IHittable> A, const std::shared_ptr<IHittable> B)
	{
		return BoxCompare(A, B, 2);
	}

	std::shared_ptr<IHittable> Left;
	std::shared_ptr<IHittable> Right;
	SAABB BoundingBox;
};



