#pragma once
#include "../Math.h"

#include <cstdint>


class OPerlin
{
public:
	OPerlin();
	~OPerlin();
	double Noise(const SVec3& P) const;
	double Turbulence(const SVec3& P, int32_t Depth = 7) const;

private:
	static int32_t* GeneratePermutation();
	static void Permute(int32_t* P, int32_t N);
	static double TrilinearInterpolation(SVec3 c[2][2][2], double U, double V, double W);
	static constexpr uint32_t PointCount = 256;
	double* RandomDouble;

	SVec3* RandomVector;
	int32_t* PermutationX;
	int32_t* PermutationY;
	int32_t* PermutationZ;
};



