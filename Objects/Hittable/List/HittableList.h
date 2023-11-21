#pragma once
#include <memory>
#include <vector>

#include "../Hittable.h"


class OHittableList final : public IHittable
{
public:
	OHittableList() = default;

	OHittableList(const std::shared_ptr<IHittable>& Object)
	{
		Objects.push_back(Object);
	}

	void Add(const std::shared_ptr<IHittable>& Object);

	void Clear();
	const auto& GetObjects() const { return Objects; }
	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const override;

	SAABB GetBoundingBox() const override;

private:
	std::vector<std::shared_ptr<IHittable>> Objects;
	SAABB BoundingBox;
};

