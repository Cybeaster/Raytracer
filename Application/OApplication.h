#pragma once
#include "../Objects/HittableList.h"
#include "../Objects/Sphere.h"
#include "../Types/Math.h"
#include "../Types/Types.h"
#include "../Utils/Materials.h"

#include "Camera/Camera.h"

class OApplication
{
public:
	OApplication();
	void AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material);

private:
	void Init();
	void Render();

	OCamera Camera;
	OHittableList World;
};





