#pragma once
#include "../Objects/Hittable/List/HittableList.h"
#include "../Objects/Hittable/Sphere/Sphere.h"
#include "../Types/Math.h"
#include "../Types/Types.h"
#include "../Utils/Materials.h"

#include "Camera/Camera.h"

class ISerializer;
class ORenderer;

class OApplication
{
public:
	static OApplication* Get()
	{
		if (Instance == nullptr)
		{
			Instance = new OApplication();
		}
		return Instance;
	}


	void AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material);
	void AddMovingSphere(const SVec3& Position, const SVec3& SecondPosition, float Radius, std::shared_ptr<IMaterial> Material);

	OCamera* GetCamera() const
	{
		return Camera.get();
	}

	OHittableList& GetWorld()
	{
		return World;
	}

private:
	OApplication();

	void Init();
	void Render();

	std::shared_ptr<OCamera> Camera;
	std::shared_ptr<ORenderer> Renderer;
	std::shared_ptr<ISerializer> Serializer;

	OHittableList World;

	static OApplication* Instance;
};





