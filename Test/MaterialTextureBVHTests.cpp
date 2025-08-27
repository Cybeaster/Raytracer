// MaterialTextureBVHTests.cpp
#include <cassert>
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>

// Project headers (adjust paths/names if needed)
#include "Objects/Materials/Lambertian.h"
#include "Objects/Materials/Metal.h"
#include "Objects/Materials/Dielectric.h"
#include "Objects/Textures/Texture.h"         // OSolidColor, ITexture base
#include "Objects/Textures/CheckerTexture.h"
#include "Objects/Hittable/BVH/BVHNode.h"
#include "Objects/Hittable/Hittable.h"
#include "Types/Math.h"
#include "Types/Interval/Interval.h"
#include "Types/AABB/AABB.h"
#include "Objects/Ray.h"

// Utilities
namespace TestUtils {
	inline bool approx(double a, double b, double eps = 1e-6) { return std::fabs(a - b) <= eps; }
	inline bool approxColor(const SColor& a, const SColor& b, double eps = 1e-6) {
		return approx(a.R, b.R, eps) && approx(a.G, b.G, eps) && approx(a.B, b.B, eps);
	}
	inline bool inHemisphere(const SVec3& v, const SVec3& n) { return Utils::Math::Dot(v, n) > 0; }
}

// Minimal controllable IHittable for BVH tests
class TControlledHittable final : public IHittable {
public:
	TControlledHittable(double tHit, const SAABB& bbox, const SVec3& normal = SVec3{ 1, 0, 0 })
		: THit(tHit), Box(bbox), N(normal) {}

	bool Hit(const SRay& ray, SInterval interval, SHitRecord& out) const override {
		if (THit < interval.Min || THit > interval.Max) return false;
		out.T = THit;
		out.Point = ray.PointAtParameter(static_cast<float>(THit));
		out.Normal = N;
		out.FrontFace = Utils::Math::Dot(ray.GetDirection(), N) < 0;
		return true;
	}
	SAABB GetBoundingBox() const override { return Box; }

private:
	double THit;
	SAABB Box;
	SVec3 N;
};

static void Test_Lambertian_ScatterHemisphere() {
	SColor albedo(0.8, 0.1, 0.1);
	OLambertian mat(albedo);

	SRay incoming(SVec3{ 0, 1, 0 }, SVec3{ 0, -1, 0 });
	SHitRecord rec;
	rec.Point = SVec3{ 0, 0, 0 };
	rec.Normal = SVec3{ 0, 1, 0 };
	rec.FrontFace = true;

	SColor attenuation;
	SRay scattered;
	const bool didScatter = mat.Scatter(incoming, rec, attenuation, scattered);

	assert(didScatter);
	assert(TestUtils::inHemisphere(scattered.GetDirection(), rec.Normal));
	assert(TestUtils::approx(attenuation.R, albedo.R));
	assert(TestUtils::approx(attenuation.G, albedo.G));
	assert(TestUtils::approx(attenuation.B, albedo.B));
}

static void Test_Metal_ReflectionAndFuzz() {
	SColor albedo(0.9, 0.9, 0.9);

	// fuzz > 1 should behave like fuzz = 1 (clamped internally)
	OMetal metal(albedo, 2.5);

	SVec3 normal{ 0, 1, 0 };
	SVec3 inDir = Utils::Math::Normalize(SVec3{ 0.3f, -1.0f, 0.1f }); // coming downward
	SRay incoming(SVec3{ 0, 0, 0 }, inDir);

	SHitRecord rec;
	rec.Point = SVec3{ 0, 0, 0 };
	rec.Normal = normal;
	rec.FrontFace = true;

	SColor attenuation;
	SRay scattered;
	const bool didScatter = metal.Scatter(incoming, rec, attenuation, scattered);

	assert(didScatter);
	// Should reflect roughly around the normal; scattered should be above the surface hemisphere
	assert(TestUtils::inHemisphere(scattered.GetDirection(), normal));
	// Attenuation equals albedo
	assert(TestUtils::approx(attenuation.R, albedo.R));
	assert(TestUtils::approx(attenuation.G, albedo.G));
	assert(TestUtils::approx(attenuation.B, albedo.B));
}

static void Test_Dielectric_RefractionAndTIR() {
	const double ior = 1.5;
	ODielectric glass(ior);

	// Entering: air -> glass (should refract or reflect probabilistically; we check valid output)
	{
		SVec3 normal{ 0, 1, 0 };
		SRay incoming(SVec3{ 0, 0, 0 }, Utils::Math::Normalize(SVec3{ 0.0f, -1.0f, 0.2f }));

		SHitRecord rec;
		rec.Point = SVec3{ 0, 0, 0 };
		rec.Normal = normal;
		rec.FrontFace = true;

		SColor attenuation;
		SRay scattered;
		const bool didScatter = glass.Scatter(incoming, rec, attenuation, scattered);
		assert(didScatter);
		// Dielectrics typically have attenuation (1,1,1)
		assert(TestUtils::approx(attenuation.R, 1.0));
		assert(TestUtils::approx(attenuation.G, 1.0));
		assert(TestUtils::approx(attenuation.B, 1.0));
	}

	// Exiting at steep angle: glass -> air, induce Total Internal Reflection
	{
		SVec3 normal{ 0, 1, 0 };
		// Ray inside glass heading upward at shallow angle relative to surface plane -> TIR
		SRay incoming(SVec3{ 0, 0, 0 }, Utils::Math::Normalize(SVec3{ 0.99f, 0.1f, 0.0f }));

		SHitRecord rec;
		rec.Point = SVec3{ 0, 0, 0 };
		rec.Normal = normal;
		rec.FrontFace = false; // from inside

		SColor attenuation;
		SRay scattered;
		const bool didScatter = glass.Scatter(incoming, rec, attenuation, scattered);
		assert(didScatter);
		// In TIR, scattered is reflective and stays in the same medium side
		// Check it's above the surface after reflection (dot with normal negative because from inside)
		assert(Utils::Math::Dot(scattered.GetDirection(), normal) < 0.0);
	}
}

static void Test_Texture_SolidColor() {
	const SColor c(0.2, 0.4, 0.6);
	auto tex = std::make_shared<OSolidColor>(c);

	const SColor v1 = tex->Value(0.0f, 0.0f, SVec3{ 1, 2, 3 });
	const SColor v2 = tex->Value(0.7f, 0.3f, SVec3{ -5, 0.5f, 10 });

	assert(TestUtils::approxColor(v1, c));
	assert(TestUtils::approxColor(v2, c));
}

static void Test_Texture_CheckerAlternation() {
	OCheckerTexture checker(1.0, SColor(1, 1, 1), SColor(0, 0, 0)); // frequency via scale

	const SColor a = checker.Value(0.0f, 0.0f, SVec3{ 0.1f, 0.1f, 0.1f });
	const SColor b = checker.Value(0.0f, 0.0f, SVec3{ 1.1f, 0.1f, 0.1f }); // shift x to flip cell

	// Expect different colors for different cells
	const bool different = !TestUtils::approxColor(a, b);
	assert(different);
}

static void Test_BVH_ClosestHitSelection() {
	// Two primitives at different distances along +X
	const SRay ray(SVec3{ -10, 0, 0 }, SVec3{ 1, 0, 0 });
	auto nearH = std::make_shared<TControlledHittable>(/*tHit*/ 2.0, SAABB(SVec3{ -1, -1, -1 }, SVec3{ 1, 1, 1 }));
	auto farH  = std::make_shared<TControlledHittable>(/*tHit*/ 5.0, SAABB(SVec3{ 10, -1, -1 }, SVec3{ 12, 1, 1 }));

	std::vector<std::shared_ptr<IHittable>> objs{ nearH, farH };
	SBVHNode bvh(objs, 0, objs.size());

	SHitRecord rec;
	const bool hit = bvh.Hit(ray, SInterval(0.001, 1e9), rec);
	assert(hit);
	assert(TestUtils::approx(rec.T, 2.0));
}

static void Test_BVH_BoundingBoxAggregation() {
	SAABB boxA(SVec3{ -2, -1, -1 }, SVec3{ -1, 1, 1 });
	SAABB boxB(SVec3{ 3, -2, -2 }, SVec3{ 5, 2, 2 });

	auto a = std::make_shared<TControlledHittable>(10.0, boxA);
	auto b = std::make_shared<TControlledHittable>(20.0, boxB);

	std::vector<std::shared_ptr<IHittable>> objs{ a, b };
	SBVHNode bvh(objs, 0, objs.size());

	const SAABB root = bvh.GetBoundingBox();
	const SVec3 rootMin = root.Min();
	const SVec3 rootMax = root.Max();

	// Root should enclose both
	using namespace Utils::Math;
	assert(GetX(rootMin) <= GetX(boxA.Min()) && GetY(rootMin) <= GetY(boxA.Min()) && GetZ(rootMin) <= GetZ(boxA.Min()));
	assert(GetX(rootMax) >= GetX(boxB.Max()) && GetY(rootMax) >= GetY(boxB.Max()) && GetZ(rootMax) >= GetZ(boxB.Max()));
}

int main() {
	std::cout << "Running Material, Texture, and BVH tests...\n";

	Test_Lambertian_ScatterHemisphere();
	Test_Metal_ReflectionAndFuzz();
	Test_Dielectric_RefractionAndTIR();

	Test_Texture_SolidColor();
	Test_Texture_CheckerAlternation();

	Test_BVH_ClosestHitSelection();
	Test_BVH_BoundingBoxAggregation();

	std::cout << "All tests passed.\n";
	return 0;
}
