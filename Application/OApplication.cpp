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


#define INIT_SCENE(Name) \
	Scenes::OScene* scene = new Name(); \
	scene->InitScene(*this); \
	delete scene;

OApplication* OApplication::Instance = nullptr;

OApplication::OApplication()
{
	Init();
	Render();
}

void OApplication::AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material)
{
	ENSURE(Material != nullptr);
	World.Add(make_shared<OSphere>(Position, Radius, Material));
}

void OApplication::AddMovingSphere(const SVec3& Position, const SVec3& SecondPosition, float Radius, std::shared_ptr<IMaterial> Material)
{
	ENSURE(Material != nullptr);
	World.Add(make_shared<OSphere>(Position, SecondPosition, Radius, Material));
}

void OApplication::AddQuad(const SVec3& Position, const SVec3& U, const SVec3& V, const std::shared_ptr<IMaterial>& Material)
{
	World.Add(make_shared<OQuad>(Position, U, V, Material));
}

void OApplication::Add(const shared_ptr<OHittableList>& List)
{
	World.Add(List);
}

void OApplication::Init()
{
	Camera = std::make_shared<OCamera>();
	INIT_SCENE(Scenes::OCornellBoxScene);
	Camera->Init();

	Serializer = std::make_shared<OPPMSerializer>(Camera->SamplesPerPixel);
	Renderer = std::make_shared<OAsyncRenderer>(Camera->ImageWidth, Camera->ImageHeight);
}

void OApplication::Render()
{
	World = { std::make_shared<SBVHNode>(World) };

	PROFILE();
	const auto& buffer = Renderer->Draw(Camera, World, Camera->SamplesPerPixel);
	Serializer->Serialize(buffer);
}
