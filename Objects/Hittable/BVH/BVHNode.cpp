//
// Created by Cybea on 21/11/2023.
//

#include "BVHNode.h"

#include <algorithm>

SBVHNode::SBVHNode(const OHittableList& List)
	: SBVHNode(List.GetObjects(), 0, List.GetObjects().size())
{
}

SBVHNode::SBVHNode(const std::vector<std::shared_ptr<IHittable>>& Objects, size_t Start, size_t End)
{
	auto objects = Objects;

	const auto axis = Utils::Math::Random(0, 2);
	const auto comparator = (axis == 0) ? BoxXCompare : (axis == 1) ? BoxYCompare : BoxZCompare;

	if (const size_t objectSpan = End - Start; objectSpan == 1)
	{
		Left = Right = objects[Start];
	}
	else if (objectSpan == 2)
	{
		if (comparator(objects[Start], objects[Start + 1]))
		{
			Left = objects[Start];
			Right = objects[Start + 1];
		}
		else
		{
			Left = objects[Start + 1];
			Right = objects[Start];
		}
	}
	// Handle the case where there are more than two objects
	else
	{
		// Sort the objects based on the comparator within the specified range
		std::sort(objects.begin() + Start, objects.begin() + End, comparator);

		// Find the middle of the range for partitioning
		auto mid = Start + objectSpan / 2;

		// Recursively create child nodes for the left and right halves of the sorted range
		Left = std::make_shared<SBVHNode>(objects, Start, mid);
		Right = std::make_shared<SBVHNode>(objects, mid, End);
	}
	// Calculate the bounding box for this node, which encloses the bounding boxes of both children
	BoundingBox = SAABB(Left->GetBoundingBox(), Right->GetBoundingBox());
}

bool SBVHNode::Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const
{
	if (!BoundingBox.Hit(Ray, Interval))
	{
		return false;
	}

	// Recursively check for intersection with the left child of the node.
	bool hitLeft = Left->Hit(Ray, Interval, HitRecord);

	// Recursively check for intersection with the right child.
	// If the left child was hit, adjust the maximum interval to the hit distance.
	bool hitRight = Right->Hit(Ray, SInterval(Interval.Min, hitLeft ? HitRecord.T : Interval.Max), HitRecord);

	return hitLeft || hitRight;
}
