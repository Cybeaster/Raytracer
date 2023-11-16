#include <iostream>
#include <fstream>
#include "OApplication.h"

#include "../Objects/Sphere.h"
#include "../Objects/SRay.h"
#include "../Objects/Materials/Lambertian.h"
#include "../Objects/Materials/Metal.h"

OApplication::OApplication()
{
	Init();
	Render();
}

void OApplication::AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material)
{
	World.Add(make_shared<OSphere>(Position, Radius, Material));
}

void OApplication::Init()
{
	auto ground = CreateMaterial<OLambertian>(SColor(0.8, 0.8, 0.0));
	auto center = CreateMaterial<OLambertian>(SColor(0.7, 0.3, 0.3));
	auto left = CreateMaterial<OMetal>(SColor(0.8, 0.8, 0.8));
	auto right = CreateMaterial<OMetal>(SColor(0.8, 0.6, 0.2));

	AddSphere(SVec3{ 0.0, -100.5, -1.0 }, 100.0, ground);

	AddSphere(SVec3{ 0.0, 0.0, -1.0 }, 0.5, center);
	AddSphere(SVec3{ -1.0, 0.0, -1.0 }, 0.5, left);
	AddSphere(SVec3{ 1.0, 0.0, -1.0 }, 0.5, right);
}

void OApplication::Render()
{
	Camera.Render(World);
}
