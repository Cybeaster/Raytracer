#pragma once
#include "../OApplication.h"


namespace Scenes
{
struct OScene
{
	virtual ~OScene() = default;
	virtual void InitScene(OApplication& OutApplication) = 0;
};


struct OFirst4SpheresScene : OScene
{
	void InitScene(OApplication& OutApplication) override
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
};

struct FOVTestScene : OScene
{
	void InitScene(OApplication& OutApplication) override
	{
		auto rad = cos(PI / 4);
		auto materialLeft = Utils::Materials::CreateMaterial<OLambertian>(SColor(0.0, 0.0, 1.0));
		auto materialRight = Utils::Materials::CreateMaterial<OLambertian>(SColor(1.0, 0.0, 0.0));

		OutApplication.AddSphere({ -rad, 0, -1 }, rad, materialLeft);
		OutApplication.AddSphere({ rad, 0, -1 }, rad, materialRight);
	}
};
}
