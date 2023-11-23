#pragma once
#include "../OApplication.h"
#include "../../Objects/Hittable/BVH/BVHNode.h"
#include "../../Objects/Materials/DiffuseLight.h"
#include "../../Objects/Textures/CheckerTexture.h"
#include "../../Objects/Textures/ImageTexture.h"
#include "../../Objects/Textures/NoiseTexture.h"


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
	auto diffuseLight = Utils::Materials::CreateMaterial<ODiffuseLight>(SColor(4, 4, 4));

	OutApplication.AddSphere({ 0, -1000, 0 }, 1000, groundMaterial);

	OutApplication.AddQuad(SVec3{ 3, 3, -2 }, SVec3{ 2, 0, 0 }, SVec3{ 0, 0, 2 }, diffuseLight);

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

	camera->SamplesPerPixel = 500;
	camera->MaxDepth = 1000;
	camera->ImageWidth = 1200;
	camera->BackgroundColor = { 0, 0, 0 };
}


DEFINE_SCENE(TwoSpheres)
{
	auto cheker = make_shared<OCheckerTexture>(0.9, SColor(0.2, 0.3, 0.1), SColor(0.9, 0.9, 0.9));
	OutApplication.AddSphere({ 0, -10, 0 }, 10, Utils::Materials::CreateMaterial<OLambertian>(cheker));
	OutApplication.AddSphere({ 0, 10, 0 }, 10, Utils::Materials::CreateMaterial<OLambertian>(cheker));

	auto camera = OutApplication.GetCamera();

	camera->DefocusAngle = 0.f;
	camera->FocusDist = 10.f;
	camera->VFov = 30;
	camera->LookFrom = { 13, 2, 3 };
	camera->LookAt = { 0, 0, 0 };

	camera->SamplesPerPixel = 40;
	camera->MaxDepth = 50;
	camera->ImageWidth = 800;
}


DEFINE_SCENE(Earth)
{
	auto earthTexture = make_shared<OImageTexture>("Earth.jpg");
	auto earthMaterial = Utils::Materials::CreateMaterial<OLambertian>(earthTexture);
	OutApplication.AddSphere({ 0, 0, 0 }, 2, earthMaterial);
	const auto camera = OutApplication.GetCamera();

	camera->DefocusAngle = 0.6f;
	camera->FocusDist = 10.f;
	camera->VFov = 20;
	camera->LookFrom = { 0, 0, 12 };
	camera->LookAt = { 0, 0, 0 };

	camera->SamplesPerPixel = 40;
	camera->MaxDepth = 50;
	camera->ImageWidth = 800;
}

DEFINE_SCENE(TwoPerlinSpheres)
{
	auto texture = make_shared<ONoiseTexture>(4);
	OutApplication.AddSphere({ 0, -1000, 0 }, 1000, Utils::Materials::CreateMaterial<OLambertian>(texture));
	OutApplication.AddSphere({ 0, 2, 0 }, 2, Utils::Materials::CreateMaterial<OLambertian>(texture));

	const auto camera = OutApplication.GetCamera();

	camera->DefocusAngle = 0.0f;
	camera->FocusDist = 10.f;
	camera->VFov = 20;
	camera->LookFrom = { 13, 2, 3 };
	camera->LookAt = { 0, 0, 0 };

	camera->SamplesPerPixel = 40;
	camera->MaxDepth = 50;
	camera->ImageWidth = 800;
}

DEFINE_SCENE(QuadTest)
{
	using namespace Utils::Materials;
	auto leftRed = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.2, 0.2));
	auto backGreen = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.2, 1.0, 0.2));
	auto rightBlue = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.2, 0.2, 1.0));
	auto upperOrange = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.5, 0.0));
	auto lowerTail = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.2, 0.8, 0.8));

	OutApplication.AddQuad(SVec3{ -3, -2, 5 }, SVec3{ 0, 0, -4 }, SVec3{ 0, 4, 0 }, leftRed);
	OutApplication.AddQuad(SVec3{ -2, -2, 0 }, SVec3{ 4, 0, 0 }, SVec3{ 0, 4, 0 }, backGreen);
	OutApplication.AddQuad(SVec3{ 3, -2, 1 }, SVec3{ 0, 0, 4 }, SVec3{ 0, 4, 0 }, rightBlue);
	OutApplication.AddQuad(SVec3{ -2, 3, 1 }, SVec3{ 4, 0, 0 }, SVec3{ 0, 0, 4 }, upperOrange);
	OutApplication.AddQuad(SVec3{ -2, -3, 5 }, SVec3{ 4, 0, 0 }, SVec3{ 0, 0, -4 }, lowerTail);

	const auto camera = OutApplication.GetCamera();
	camera->AspectRatio = 1.0;
	camera->DefocusAngle = 0.0f;
	camera->FocusDist = 10.f;
	camera->VFov = 80;
	camera->LookFrom = { 0, 0, 9 };
	camera->LookAt = { 0, 0, 0 };

	camera->SamplesPerPixel = 40;
	camera->MaxDepth = 50;
	camera->ImageWidth = 800;
}

DEFINE_SCENE(OneLightTest)
{
	auto pertex = make_shared<ONoiseTexture>(4);
	OutApplication.AddSphere(SVec3{ 0, -1000, 0 }, 1000, Utils::Materials::CreateMaterial<OLambertian>(pertex));
	OutApplication.AddSphere(SVec3{ 0, 2, 0 }, 2, Utils::Materials::CreateMaterial<OLambertian>(pertex));

	auto diffLight = make_shared<ODiffuseLight>(SColor(4, 4, 4));
	OutApplication.AddQuad(SVec3{ 3, 1, -2 }, SVec3{ 2, 0, 0 }, SVec3{ 0, 2, 0 }, diffLight);

	auto camera = OutApplication.GetCamera();
	camera->BackgroundColor = { 0, 0, 0 };

	camera->DefocusAngle = 0.0f;
	camera->FocusDist = 10.f;
	camera->VFov = 20;
	camera->LookFrom = { 26, 3, 6 };
	camera->LookAt = { 0, 2, 0 };

	camera->SamplesPerPixel = 100;
	camera->MaxDepth = 50;
	camera->ImageWidth = 800;
}


DEFINE_SCENE(CornellBox)
{
	auto red = IMaterial::CreateMaterial<OLambertian>(SColor(0.65, 0.05, 0.05));
	auto white = IMaterial::CreateMaterial<OLambertian>(SColor(0.73, 0.73, 0.73));
	auto green = IMaterial::CreateMaterial<OLambertian>(SColor(0.12, 0.45, 0.15));
	auto light = IMaterial::CreateMaterial<ODiffuseLight>(SColor(15, 15, 15));

	OutApplication.AddQuad(SVec3{ 555, 0, 0 }, SVec3{ 0, 0, 555 }, SVec3{ 0, 555, 0 }, green);
	OutApplication.AddQuad(SVec3{ 0, 0, 0 }, SVec3{ 0, 555, 0 }, SVec3{ 0, 0, 555 }, red);
	OutApplication.AddQuad(SVec3{ 343, 555, 332 }, SVec3{ -130, 0, 0 }, SVec3{ 0, 0, 555 }, light);
	OutApplication.AddQuad(SVec3{ 0, 0, 0 }, SVec3{ 555, 0, 0 }, SVec3{ 0, 0, 555 }, white);
	OutApplication.AddQuad(SVec3{ 555, 555, 555 }, SVec3{ -555, 0, 0 }, SVec3{ 0, 0, -555 }, white);
	OutApplication.AddQuad(SVec3{ 0, 0, 555 }, SVec3{ 555, 0, 0 }, SVec3{ 0, 555, 0 }, white);

	OutApplication.Add(OHittableList::CreateBox({ 130, 0, 65 }, { 295, 165, 230 }, white));
	OutApplication.Add(OHittableList::CreateBox({ 265, 0, 295 }, { 430, 330, 460 }, white));

	auto camera = OutApplication.GetCamera();

	camera->BackgroundColor = { 0, 0, 0 };
	camera->DefocusAngle = 0.0f;
	camera->FocusDist = 10.f;
	camera->VFov = 40;
	camera->LookFrom = { 278, 278, -800 };
	camera->LookAt = { 278, 278, 0 };
	camera->AspectRatio = 1.0;
	camera->SamplesPerPixel = 200;
	camera->MaxDepth = 50;
	camera->ImageWidth = 600;
}
}
