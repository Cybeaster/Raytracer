#pragma once
#include "../Objects/Hittable/List/HittableList.h"
#include "../Objects/Hittable/Sphere/Sphere.h"
#include "../Types/Logger.h"
#include "../Types/Math.h"
#include "../Types/Types.h"
#include "../Utils/Materials.h"

#include "Camera/Camera.h"

#include <iostream>

namespace Scenes
{
struct OScene;
}

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

	OCamera* GetCamera() const
	{
		return Camera.get();
	}

	OHittableList& GetWorld()
	{
		return World;
	}

	void AddScene(const string& Name, Scenes::OScene* Scene);
	void RenderScene(const string& Name, ECameraPresets CameraPreset = ECameraPresets::Medium);

private:
	void InitRenderer();
	void InitScene(ECameraPresets CameraPreset);
	void Render();

	std::shared_ptr<OCamera> Camera;
	std::shared_ptr<ORenderer> Renderer;
	std::shared_ptr<ISerializer> Serializer;

	OHittableList World;
	Scenes::OScene* CurrentScene = nullptr;
	unordered_map<string, Scenes::OScene*> AllScenes;
	static OApplication* Instance;
};





