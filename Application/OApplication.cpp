#include "OApplication.h"

#include "../Objects/Hittable/Quad/Quad.h"
#include "../Objects/Materials/Dielectric.h"
#include "../Objects/Hittable/Sphere/Sphere.h"
#include "../Objects/Materials/Lambertian.h"
#include "../Objects/Materials/Metal.h"
#include "../Types/Output/PPMSerializer.h"
#include "../Types/Profiler/Profiler.h"
#include "../Utils/Materials.h"
#include "Renderers/AsyncRenderer.h"
#include "Renderers/Renderer.h"
#include "Renderers/SyncRenderer.h"
#include "Scenes\Scenes.h"


OApplication* OApplication::Instance = nullptr;

void OApplication::AddScene(const string& Name, Scenes::OScene* Scene)
{
	if (AllScenes.contains(Name))
	{
		LOG(Error, "Scene with name {} already exists", Name);
	}
	AllScenes[Name] = Scene;
}

void OApplication::RenderScene(const string& Name)
{
	if (AllScenes.contains(Name) == false)
	{
		LOG(Error, "Scene with name {} does not exist", Name);
		return;
	}

	LOG(Log, "Rendering scene: {}", Name);
	CurrentScene = AllScenes[Name];

	InitRenderer();
	InitScene();
	Render();

	LOG(Log, "Rendered scene: {}", CurrentScene->SceneName);
}

void OApplication::InitScene()
{
	CurrentScene->SetObjects(&World, Camera.get());
	CurrentScene->InitScene();

	Camera->Init();
	Serializer->Init(Camera->SamplesPerPixel);
	Renderer->Init(Camera->ImageWidth, Camera->ImageHeight);
}

void OApplication::InitRenderer()
{
	Camera = std::make_shared<OCamera>();
	Serializer = std::make_shared<OPPMSerializer>();
	Renderer = std::make_shared<OAsyncRenderer>();
}

void OApplication::Render()
{
	PROFILE(CurrentScene->SceneName);

	//Optimize wordl with BVH nodes
	World = { std::make_shared<SBVHNode>(World) };

	const auto& buffer = Renderer->Draw(Camera, World, Camera->SamplesPerPixel);
	Serializer->Serialize(buffer, CurrentScene->SceneName);
}
