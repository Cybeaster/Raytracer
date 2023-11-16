#pragma once
#include "../Objects/HittableList.h"
#include "../Objects/Sphere.h"
#include "../Types/Math.h"
#include "../Types/Types.h"

#include "Camera/Camera.h"

class OApplication
{
public:
	OApplication();

private:
	void AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material);

	template<typename MatType>
	auto CreateMaterial(SColor Color) const;

	void Init();

	void Render();

	OCamera Camera;

	OHittableList World;
};


template<typename MatType>
auto OApplication::CreateMaterial(SColor Color) const
{
	return make_shared<MatType>(Color);
}


