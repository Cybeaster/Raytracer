#pragma once

#include "../Types/Math.h"
#include "../Ray/ORay.h"

// Constants
const float Infinity = std::numeric_limits<float>::infinity();
constexpr float PI = 3.1415926535897932385;

namespace Utils { namespace Math
	{
	inline auto Lerp(float T, const SVec3& A, const SVec3& B)
	{
		return (1.0f - T) * A + T * B; // The interpolated vector
	}

	inline auto Normalize(const SVec3& Vec)
	{
		return normalized(Vec); // Return the normalized vector
	}

	inline auto Dot(const SVec3& A, const SVec3& B)
	{
		return dot(A, B); // Return the dot product
	}

	inline auto LengthSquared(const SVec3& A)
	{
		return mag_sqr(A); // Return the squared magnitude of the vector
	}

	inline auto Magnitude(const SVec3& A)
	{
		return boost::qvm::mag(A); // Return the magnitude of the vector
	}

	inline auto IsNormalized(const SVec3& A, float Epsilon = 1e-6f)
	{
		return abs(1.0f - Magnitude(A)) < Epsilon; // Return true if the vector is normalized
	}

	template<typename T>
	auto Squared(T Value)
	{
		return Value * Value;
	}

	inline auto DegreesToRaians(auto Degrees)
	{
		return Degrees * (PI / 180.0f);
	}
	}


	inline auto SphereIntersection(const SVec3& Center, float Radius, const ORay& Ray)
	{
		using namespace Math;
		auto oc = Ray.GetOrigin() - Center; // Vector from sphere center to ray origin
		auto a = LengthSquared(Ray.GetDirection()); // Squared magnitude of the ray's direction
		auto halfB = Dot(oc, Ray.GetDirection());
		auto c = LengthSquared(oc) - Squared(Radius);
		auto discriminant = Squared(halfB) - a * c;
		if (discriminant < 0)
		{
			return -1.0f; // No intersection, return -1
		}
		else
		{
			return float((-halfB - sqrt(discriminant)) / a); // Return the smaller root of the quadratic equation
		}
	}
}
