#pragma once
#include "../OApplication.h"
#include "../../Objects/Hittable/BVH/BVHNode.h"
#include "../../Objects/Hittable/Translated/Translated.h"
#include "../../Objects/Hittable/Volumes/ConstantMediumVolume.h"
#include "../../Objects/Hittable/YRotated/YRotated.h"
#include "../../Objects/Materials/DiffuseLight.h"
#include "../../Objects/Textures/CheckerTexture.h"
#include "../../Objects/Textures/ImageTexture.h"
#include "../../Objects/Textures/NoiseTexture.h"


#define DEFINE_SCENE(Name,...) \
	struct O##Name##Scene : OScene \
	{\
		O##Name##Scene() : OScene(#Name)\
		{\
			OApplication::Get()->AddScene(SceneName, this);\
		}\
		void InitScene() override; \
		\
	}inline Name;\
	\
	inline void O##Name##Scene::InitScene()


namespace Scenes
{
struct OScene
{
	OScene(string Name)
		: SceneName(std::move(Name))
	{
	}

	virtual ~OScene() = default;
	virtual void InitScene() = 0;

	void SetObjects(OHittableList* W, OCamera* C)
	{
		Camera = C;
		World = W;
	}

	void AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material) const
	{
		ENSURE(Material != nullptr);
		World->Add(make_shared<OSphere>(Position, Radius, Material));
	}

	void AddMovingSphere(const SVec3& Position, const SVec3& SecondPosition, float Radius, std::shared_ptr<IMaterial> Material) const
	{
		ENSURE(Material != nullptr);
		World->Add(make_shared<OSphere>(Position, SecondPosition, Radius, Material));
	}

	void AddQuad(const SVec3& Position, const SVec3& U, const SVec3& V, const std::shared_ptr<IMaterial>& Material) const
	{
		World->Add(make_shared<OQuad>(Position, U, V, Material));
	}

	void Add(const shared_ptr<IHittable>& List) const
	{
		World->Add(List);
	}

	static auto CreateBox(const SVec3& A, const SVec3& B, const shared_ptr<IMaterial>& Material)
	{
		return OHittableList::CreateBox(A, B, Material);
	}

public:
	OHittableList* World = nullptr;
	OCamera* Camera = nullptr;
	string SceneName;
};


DEFINE_SCENE(First4Spheres)
{
	auto ground = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.8, 0.8, 0.0));
	auto center = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.1, 0.2, 0.5));
	auto left = Utils::Materials::CreateMaterial<ODielectric>(1.5);
	auto right = Utils::Materials::CreateMaterial<OMetal>(SColor(1, 0, 0), 0.1);

	AddSphere(SVec3{ 0.0, -100.5, -1.0 }, 100.0, ground);
	AddSphere(SVec3{ 0.0, 0.0, -1.0 }, 0.5, center);
	AddSphere(SVec3{ -1.0, 0.0, -1.0 }, -0.5, left);
	AddSphere(SVec3{ 1.0, 0.0, -1.0 }, 0.5, right);
}

DEFINE_SCENE(FOVTest)
{
	auto rad = cos(PI / 4);
	auto materialLeft = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.0, 0.0, 1.0));
	auto materialRight = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.0, 0.0));

	AddSphere({ -rad, 0, -1 }, rad, materialLeft);
	AddSphere({ rad, 0, -1 }, rad, materialRight);
}

DEFINE_SCENE(LotsRandomSpheres)
{
	auto checker = make_shared<OCheckerTexture>(0.5, SColor(1, 0, 0), SColor(0.9, 0.9, 0.9));

	auto groundMaterial = Utils::Materials::CreateMaterial<OLambertian>(checker);
	auto diffuseLight = Utils::Materials::CreateMaterial<ODiffuseLight>(SColor(4, 4, 4));

	AddSphere({ 0, -1000, 0 }, 1000, groundMaterial);

	AddQuad(SVec3{ 3, 3, -2 }, SVec3{ 2, 0, 0 }, SVec3{ 0, 0, 2 }, diffuseLight);

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
					AddSphere(center, 0.2, sphereMaterial);
				}
				else if (chooseMat < 0.95)
				{
					auto albedo = SColor::Random(0.5, 1);
					auto fuzz = Utils::Math::Random<double>(0, 0.5);
					sphereMaterial = Utils::Materials::CreateMaterial<OMetal>(albedo, fuzz);
					auto point2 = center + SVec3{ 0, Utils::Math::Random<float>(0, 0.5), 0 };
					AddMovingSphere(center, point2, 0.2, sphereMaterial);
				}
				else
				{
					sphereMaterial = Utils::Materials::CreateMaterial<ODielectric>(1.5);
					AddSphere(center, 0.2, sphereMaterial);
				}
			}
		}
	}
	const auto mat1 = Utils::Materials::CreateMaterial<ODielectric>(1.5);
	const auto mat1Pos = SVec3{ 0, 1, 0 };

	AddMovingSphere(mat1Pos,
	                mat1Pos + SVec3{ 0, Utils::Math::Random<float>(0, 0.5), 0 },
	                1.0,
	                mat1);

	auto mat2 = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.4, 0.2, 0.1));
	AddSphere({ -4, 1, 0 }, 1.0, mat2);

	auto mat3 = Utils::Materials::CreateMaterial<OMetal>(SColor(0.7, 0.6, 0.5), 0.0);
	AddSphere({ 4, 1, 0 }, 1.0, mat3);

	Camera->DefocusAngle = 0.6f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 30;
	Camera->LookFrom = { 13, 2, 3 };
	Camera->LookAt = { 0, 0, 0 };

	Camera->BackgroundColor = { 0, 0, 0 };
}


DEFINE_SCENE(TwoSpheres)
{
	auto cheker = make_shared<OCheckerTexture>(0.9, SColor(0.2, 0.3, 0.1), SColor(0.9, 0.9, 0.9));
	AddSphere({ 0, -10, 0 }, 10, Utils::Materials::CreateMaterial<OLambertian>(cheker));
	AddSphere({ 0, 10, 0 }, 10, Utils::Materials::CreateMaterial<OLambertian>(cheker));

	Camera->DefocusAngle = 0.f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 30;
	Camera->LookFrom = { 13, 2, 3 };
	Camera->LookAt = { 0, 0, 0 };
}


DEFINE_SCENE(Earth)
{
	auto earthTexture = make_shared<OImageTexture>("Earth.jpg");
	auto earthMaterial = Utils::Materials::CreateMaterial<OLambertian>(earthTexture);
	AddSphere({ 0, 0, 0 }, 2, earthMaterial);

	Camera->DefocusAngle = 0.6f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 20;
	Camera->LookFrom = { 0, 0, 12 };
	Camera->LookAt = { 0, 0, 0 };
}

DEFINE_SCENE(TwoPerlinSpheres)
{
	auto texture = make_shared<ONoiseTexture>(4);
	AddSphere({ 0, -1000, 0 }, 1000, Utils::Materials::CreateMaterial<OLambertian>(texture));
	AddSphere({ 0, 2, 0 }, 2, Utils::Materials::CreateMaterial<OLambertian>(texture));

	Camera->DefocusAngle = 0.0f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 20;
	Camera->LookFrom = { 13, 2, 3 };
	Camera->LookAt = { 0, 0, 0 };
}

DEFINE_SCENE(QuadTest)
{
	using namespace Utils::Materials;
	auto leftRed = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.2, 0.2));
	auto backGreen = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.2, 1.0, 0.2));
	auto rightBlue = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.2, 0.2, 1.0));
	auto upperOrange = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.5, 0.0));
	auto lowerTail = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.2, 0.8, 0.8));

	AddQuad(SVec3{ -3, -2, 5 }, SVec3{ 0, 0, -4 }, SVec3{ 0, 4, 0 }, leftRed);
	AddQuad(SVec3{ -2, -2, 0 }, SVec3{ 4, 0, 0 }, SVec3{ 0, 4, 0 }, backGreen);
	AddQuad(SVec3{ 3, -2, 1 }, SVec3{ 0, 0, 4 }, SVec3{ 0, 4, 0 }, rightBlue);
	AddQuad(SVec3{ -2, 3, 1 }, SVec3{ 4, 0, 0 }, SVec3{ 0, 0, 4 }, upperOrange);
	AddQuad(SVec3{ -2, -3, 5 }, SVec3{ 4, 0, 0 }, SVec3{ 0, 0, -4 }, lowerTail);

	Camera->AspectRatio = 1.0;
	Camera->DefocusAngle = 0.0f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 80;
	Camera->LookFrom = { 0, 0, 9 };
	Camera->LookAt = { 0, 0, 0 };
}

DEFINE_SCENE(OneLightTest)
{
	auto pertex = make_shared<ONoiseTexture>(4);
	AddSphere(SVec3{ 0, -1000, 0 }, 1000, Utils::Materials::CreateMaterial<OLambertian>(pertex));
	AddSphere(SVec3{ 0, 2, 0 }, 2, Utils::Materials::CreateMaterial<OLambertian>(pertex));

	auto diffLight = make_shared<ODiffuseLight>(SColor(4, 4, 4));
	AddQuad(SVec3{ 3, 1, -2 }, SVec3{ 2, 0, 0 }, SVec3{ 0, 2, 0 }, diffLight);

	Camera->BackgroundColor = { 0, 0, 0 };

	Camera->DefocusAngle = 0.0f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 20;
	Camera->LookFrom = { 26, 3, 6 };
	Camera->LookAt = { 0, 2, 0 };
}


DEFINE_SCENE(CornellBox)
{
	auto red = IMaterial::CreateMaterial<OLambertian>(SColor(0.65, 0.05, 0.05));
	auto white = IMaterial::CreateMaterial<OLambertian>(SColor(0.73, 0.73, 0.73));
	auto green = IMaterial::CreateMaterial<OLambertian>(SColor(0.12, 0.45, 0.15));
	auto light = IMaterial::CreateMaterial<ODiffuseLight>(SColor(15, 15, 15));

	AddQuad(SVec3{ 555, 0, 0 }, SVec3{ 0, 0, 555 }, SVec3{ 0, 555, 0 }, green);
	AddQuad(SVec3{ 0, 0, 0 }, SVec3{ 0, 555, 0 }, SVec3{ 0, 0, 555 }, red);
	AddQuad(SVec3{ 343, 555, 332 }, SVec3{ -130, 0, 0 }, SVec3{ 0, 0, 555 }, light);
	AddQuad(SVec3{ 0, 0, 0 }, SVec3{ 555, 0, 0 }, SVec3{ 0, 0, 555 }, white);
	AddQuad(SVec3{ 555, 555, 555 }, SVec3{ -555, 0, 0 }, SVec3{ 0, 0, -555 }, white);
	AddQuad(SVec3{ 0, 0, 555 }, SVec3{ 555, 0, 0 }, SVec3{ 0, 555, 0 }, white);

	shared_ptr<IHittable> box1 = CreateBox({ 0, 0, 0 }, { 165, 330, 165 }, white);
	box1 = make_shared<ORotatedY>(box1, 15);
	box1 = make_shared<OTranslated>(box1, SVec3{ 265, 0, 295 });
	Add(box1);

	shared_ptr<IHittable> box2 = CreateBox({ 0, 0, 0 }, { 165, 165, 165 }, white);
	box2 = make_shared<ORotatedY>(box2, -18);
	box2 = make_shared<OTranslated>(box2, SVec3{ 130, 0, 65 });
	Add(box2);

	Camera->BackgroundColor = { 0, 0, 0 };
	Camera->DefocusAngle = 0.0f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 40;
	Camera->LookFrom = { 278, 278, -800 };
	Camera->LookAt = { 278, 278, 0 };
	Camera->AspectRatio = 1.0;
}


DEFINE_SCENE(CornelSmoke)
{
	auto red = IMaterial::CreateMaterial<OLambertian>(SColor(0.65, 0.05, 0.05));
	auto white = IMaterial::CreateMaterial<OLambertian>(SColor(0.73, 0.73, 0.73));
	auto green = IMaterial::CreateMaterial<OLambertian>(SColor(0.12, 0.45, 0.15));
	auto light = IMaterial::CreateMaterial<ODiffuseLight>(SColor(15, 15, 15));

	AddQuad(SVec3{ 555, 0, 0 }, SVec3{ 0, 0, 555 }, SVec3{ 0, 555, 0 }, green);
	AddQuad(SVec3{ 0, 0, 0 }, SVec3{ 0, 555, 0 }, SVec3{ 0, 0, 555 }, red);
	AddQuad(SVec3{ 343, 555, 332 }, SVec3{ -130, 0, 0 }, SVec3{ 0, 0, 555 }, light);
	AddQuad(SVec3{ 0, 0, 0 }, SVec3{ 555, 0, 0 }, SVec3{ 0, 0, 555 }, white);
	AddQuad(SVec3{ 555, 555, 555 }, SVec3{ -555, 0, 0 }, SVec3{ 0, 0, -555 }, white);
	AddQuad(SVec3{ 0, 0, 555 }, SVec3{ 555, 0, 0 }, SVec3{ 0, 555, 0 }, white);

	shared_ptr<IHittable> box1 = CreateBox({ 0, 0, 0 }, { 165, 330, 165 }, white);
	box1 = make_shared<ORotatedY>(box1, 15);
	box1 = make_shared<OTranslated>(box1, SVec3{ 265, 0, 295 });
	Add(make_shared<OConstantMediumVolume>(box1, 0.01, SColor(0, 0, 0)));

	shared_ptr<IHittable> box2 = CreateBox({ 0, 0, 0 }, { 165, 165, 165 }, white);
	box2 = make_shared<ORotatedY>(box2, -18);
	box2 = make_shared<OTranslated>(box2, SVec3{ 130, 0, 65 });
	Add(make_shared<OConstantMediumVolume>(box2, 0.01, SColor(1, 1, 1)));

	Camera->BackgroundColor = { 0, 0, 0 };
	Camera->DefocusAngle = 0.0f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 40;
	Camera->LookFrom = { 278, 278, -800 };
	Camera->LookAt = { 278, 278, 0 };
	Camera->AspectRatio = 1.0;
}

DEFINE_SCENE(ManyObjectsSmoke)
{
	auto ground = IMaterial::CreateMaterial<OLambertian>(SColor(0.48, 0.83, 0.53));
	int32_t boxesPerSide = 20;

	OHittableList list;
	for (int i = 0; i < boxesPerSide; i++)
	{
		for (int j = 0; j < boxesPerSide; j++)
		{
			float w = 100.0;
			float x0 = -1000.0 + i * w;
			float z0 = -1000.0 + j * w;
			float y0 = 0.0;

			float x1 = x0 + w;
			float y1 = Utils::Math::Random(1, 101);
			float z1 = z0 + w;

			list.Add(CreateBox(SVec3{ x0, y0, z0 }, SVec3{ x1, y1, z1 }, ground));
		}
	}
	Add(make_shared<SBVHNode>(list));

	auto light = make_shared<ODiffuseLight>(SColor(7, 7, 7));
	Add(make_shared<OQuad>(SVec3{ 123, 554, 147 }, SVec3{ 300, 0, 0 }, SVec3{ 0, 0, 265 }, light));

	auto center1 = SVec3{ 400, 400, 200 };
	auto center2 = center1 + SVec3{ 30, 0, 0 };
	auto sphereMaterial = IMaterial::CreateMaterial<OLambertian>(SColor{ 0.7, 0.3, 0.1 });

	AddMovingSphere(center1, center2, 50, sphereMaterial);
	AddSphere(SVec3{ 260, 150, 45 }, 50, IMaterial::CreateMaterial<ODielectric>(1.5));
	AddSphere(SVec3{ 0, 150, 145 }, 50, IMaterial::CreateMaterial<OMetal>(SColor{ 0.8, 0.8, 0.9 }, 1.0));

	auto boundary = make_shared<OSphere>(SVec3{ 360, 150, 145 }, 70, IMaterial::CreateMaterial<ODielectric>(1.5));
	Add(boundary);
	Add(make_shared<OConstantMediumVolume>(boundary, 0.2, SColor{ 0.2, 0.4, 0.9 }));
	boundary = make_shared<OSphere>(SVec3{ 0, 0, 0 }, 5000, IMaterial::CreateMaterial<ODielectric>(1.5));
	Add(make_shared<OConstantMediumVolume>(boundary, 0.0001, SColor{ 1, 1, 1 }));

	auto emat = IMaterial::CreateMaterial<OLambertian>(make_shared<OImageTexture>("Earth.jpg"));
	AddSphere(SVec3{ 400, 200, 400 }, 100, emat);
	auto pertex = make_shared<ONoiseTexture>(0.1);
	AddSphere(SVec3{ 220, 280, 300 }, 80, IMaterial::CreateMaterial<OLambertian>(pertex));

	OHittableList boxes2;

	auto white = IMaterial::CreateMaterial<OLambertian>(SColor(0.73, 0.73, 0.73));
	int32_t ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxes2.Add(make_shared<OSphere>(Utils::Math::RandomVec(0, 165), 10, white));
	}

	const auto boxes = make_shared<OTranslated>(make_shared<ORotatedY>(make_shared<OHittableList>(make_shared<SBVHNode>(boxes2)), 15), SVec3{ -100, 270, 395 });

	Add(boxes);
	Camera->BackgroundColor = { 0, 0, 0 };
	Camera->DefocusAngle = 0.0f;
	Camera->FocusDist = 10.f;
	Camera->VFov = 40;
	Camera->LookFrom = { 478, 278, -600 };
	Camera->LookAt = { 278, 278, 0 };
	Camera->AspectRatio = 1.0;
}
}
