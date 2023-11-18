#include "OApplication.h"
#include "../Objects/Materials/Dielectric.h"
#include "../Objects/Sphere.h"
#include "../Objects/Materials/Lambertian.h"
#include "../Objects/Materials/Metal.h"
#include "../Types/Output/PPMSerializer.h"
#include "../Types/Profiler/Profiler.h"
#include "../Utils/Materials.h"
#include "Renderers/AsyncRenderer.h"
#include "Renderers/Renderer.h"
#include "Renderers/SyncRenderer.h"
#include "Scencens/Scences.h"

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

void OApplication::Init()
{
	Scenes::FOVTestScene FOVScene;
	Scenes::OFirst4SpheresScene FirstScene;

	Camera = std::make_shared<OCamera>();
	Serializer = std::make_shared<OPPMSerializer>(Camera->SamplesPerPixel);
	Renderer = std::make_shared<OAsyncRenderer>(Camera->ImageWidth, Camera->ImageHeight);

	FirstScene.InitScene(*this);

	Camera->DefocusAngle = 15;
	Camera->FocusDist = 3.4f;
	Camera->VFov = 45;
	Camera->ImageWidth = 800;
}

void OApplication::Render()
{
	PROFILE();
	const auto& buffer = Renderer->Draw(Camera, World, Camera->SamplesPerPixel);
	Serializer->Serialize(buffer);
}
