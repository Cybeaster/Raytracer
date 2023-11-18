#pragma once
#include "../Objects/HittableList.h"
#include "../Objects/Sphere.h"
#include "../Types/Math.h"
#include "../Types/Types.h"
#include "../Utils/Materials.h"

#include "Camera/Camera.h"

class ISerializer;
class ORenderer;

class OApplication
{
public:
	OApplication();
	void AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material);

private:
	void Init();
	void Render();

	std::shared_ptr<OCamera> Camera;
	std::shared_ptr<ORenderer> Renderer;
	std::shared_ptr<ISerializer> Serializer;

	OHittableList World;
};





