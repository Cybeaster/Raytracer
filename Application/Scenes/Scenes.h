#pragma once
#include "../OApplication.h"
#include "../../Objects/Hittable/BVH/BVHNode.h"
#include "../../Objects/Textures/CheckerTexture.h"


#define DEFINE_SCENE(Name) \
	struct O##Name##Scene : OScene \
	{\
		O##Name##Scene()\
		{\
			std::cout << "Running " << #Name << " scene" << std::endl;\
		}\
		void InitScene(OApplication& OutApplication) override; \
	};\
	void O##Name##Scene::InitScene(OApplication& OutApplication)


namespace Scenes
{
struct OScene
{
	virtual ~OScene() = default;
	virtual void InitScene(OApplication& OutApplication) = 0;
};


DEFINE_SCENE(First4Spheres)
{
	auto ground = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.8, 0.8, 0.0));
	auto center = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.1, 0.2, 0.5));
	auto left = Utils::Materials::CreateMaterial<ODielectric>(1.5);
	auto right = Utils::Materials::CreateMaterial<OMetal>(SColor(1, 0, 0), 0.1);

	OutApplication.AddSphere(SVec3{ 0.0, -100.5, -1.0 }, 100.0, ground);
	OutApplication.AddSphere(SVec3{ 0.0, 0.0, -1.0 }, 0.5, center);
	OutApplication.AddSphere(SVec3{ -1.0, 0.0, -1.0 }, -0.5, left);
	OutApplication.AddSphere(SVec3{ 1.0, 0.0, -1.0 }, 0.5, right);
}

DEFINE_SCENE(FOVTest)
{
	auto rad = cos(PI / 4);
	auto materialLeft = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.0, 0.0, 1.0));
	auto materialRight = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.0, 0.0));

	OutApplication.AddSphere({ -rad, 0, -1 }, rad, materialLeft);
	OutApplication.AddSphere({ rad, 0, -1 }, rad, materialRight);
}

DEFINE_SCENE(LotsRandomSpheres)
{
	auto checker = make_shared<OCheckerTexture>(0.5, SColor(1, 0, 0), SColor(0.9, 0.9, 0.9));
	auto groundMaterial = Utils::Materials::CreateMaterial<OLambertian>(checker);
	OutApplication.AddSphere({ 0, -1000, 0 }, 1000, groundMaterial);

	for (int i = -11; i < 11; i++)
	{
		for (int j = -11; j < 11; j++)
		{
			auto chooseMat = Utils::Math::Random<double>();
			SVec3 center{ i + 0.9f * Utils::Math::Random<float>(), 0.2, j + 0.9f * Utils::Math::Random<float>() };
			if (Utils::Math::Length(center - SVec3{ 4.f, 0.2f, 0.f }) > 0.9)
			{
				std::shared_ptr<IMaterial> sphereMaterial;
				if (chooseMat < 0.8)
				{
					auto albedo = SColor::Random() * SColor::Random();
					sphereMaterial = Utils::Materials::CreateMaterial<OLambertian>(albedo);
					OutApplication.AddSphere(center, 0.2, sphereMaterial);
				}
				else if (chooseMat < 0.95)
				{
					auto albedo = SColor::Random(0.5, 1);
					auto fuzz = Utils::Math::Random<double>(0, 0.5);
					sphereMaterial = Utils::Materials::CreateMaterial<OMetal>(albedo, fuzz);
					auto point2 = center + SVec3{ 0, Utils::Math::Random<float>(0, 0.5), 0 };
					OutApplication.AddMovingSphere(center, point2, 0.2, sphereMaterial);
				}
				else
				{
					sphereMaterial = Utils::Materials::CreateMaterial<ODielectric>(1.5);
					OutApplication.AddSphere(center, 0.2, sphereMaterial);
				}
			}
		}
	}
	const auto mat1 = Utils::Materials::CreateMaterial<ODielectric>(1.5);
	const auto mat1Pos = SVec3{ 0, 1, 0 };

	OutApplication.AddMovingSphere(mat1Pos,
	                               mat1Pos + SVec3{ 0, Utils::Math::Random<float>(0, 0.5), 0 },
	                               1.0,
	                               mat1);

	auto mat2 = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.4, 0.2, 0.1));
	OutApplication.AddSphere({ -4, 1, 0 }, 1.0, mat2);

	auto mat3 = Utils::Materials::CreateMaterial<OMetal>(SColor(0.7, 0.6, 0.5), 0.0);
	OutApplication.AddSphere({ 4, 1, 0 }, 1.0, mat3);

	const auto camera = OutApplication.GetCamera();

	camera->DefocusAngle = 0.6f;
	camera->FocusDist = 10.f;
	camera->VFov = 30;
	camera->LookFrom = { 13, 2, 3 };
	camera->LookAt = { 0, 0, 0 };

	camera->SamplesPerPixel = 40;
	camera->MaxDepth = 50;
	camera->ImageWidth = 800;
}
}
