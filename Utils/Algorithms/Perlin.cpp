//
// Created by Cybea on 22/11/2023.
//

#include "Perlin.h"

OPerlin::OPerlin()
{
	using namespace Utils::Math;
	RandomVector = new SVec3[PointCount];

	for (int i = 0; i < PointCount; i++)
	{
		RandomVector[i] = UnitVector(RandomVec(-1, 1));
	}

	RandomDouble = new double[PointCount];
	for (uint32_t i = 0; i < PointCount; ++i)
	{
		RandomDouble[i] = Utils::Math::Random<double>();
	}

	PermutationX = GeneratePermutation();
	PermutationY = GeneratePermutation();
	PermutationZ = GeneratePermutation();
}

OPerlin::~OPerlin()
{
	delete[] RandomDouble;
	delete[] PermutationX;
	delete[] PermutationY;
	delete[] PermutationZ;
	delete[] RandomVector;
}

double OPerlin::Noise(const SVec3& P) const
{
	using namespace Utils::Math;

	//Take fractional part
	auto u = GetX(P) - floor(GetX(P));
	auto v = GetY(P) - floor(GetY(P));
	auto w = GetZ(P) - floor(GetZ(P));

	//Take real part
	auto i = static_cast<int32_t>(floor(GetX(P)));
	auto j = static_cast<int32_t>(floor(GetY(P)));
	auto k = static_cast<int32_t>(floor(GetZ(P)));
	// A 2x2x2 array to store values at the corners of the grid cell.
	SVec3 c[2][2][2];

	for (int di = 0; di < 2; di++)
	{
		for (int dj = 0; dj < 2; dj++)
		{
			for (int dk = 0; dk < 2; dk++)
			{
				// Calculate a pseudo-random value for each corner of the cell.
				// The permutations and bitwise operations are used to get a consistent but seemingly random value.

				c[di][dj][dk] = RandomVector[
					PermutationX[(i + di) & 255] ^
					PermutationY[(j + dj) & 255] ^
					PermutationZ[(k + dk) & 255]];
			}
		}
	}
	return TrilinearInterpolation(c, u, v, w);
}

double OPerlin::Turbulence(const SVec3& P, int32_t Depth) const
{
	// Initialize the accumulator for the final turbulence value.
	auto accum = 0.0;

	// Temporary point, starting at P, to be scaled in each iteration.
	auto tempP = P;

	// Initial weight for scaling the noise contribution.
	auto weight = 1.0;

	// Iterate over the number of octaves specified by Depth.
	for (int i = 0; i < Depth; i++)
	{
		// Add the scaled noise value to the accumulator.
		// 'Noise' function generates Perlin noise value for the point.
		accum += weight * Noise(tempP);

		// Reduce the weight by half for the next octave.
		weight *= 0.5;

		// Scale the point by a factor of 2 for the next octave.
		// This increases the frequency of the noise function.
		tempP *= 2;
	}

	// Return the absolute value of the accumulated noise.
	// This ensures the turbulence value is non-negative.
	return fabs(accum);
}

int32_t* OPerlin::GeneratePermutation()
{
	const auto p = new int32_t[PointCount];
	for (int i = 0; i < PointCount; ++i)
	{
		p[i] = i;
	}
	Permute(p, PointCount);
	return p;
}

void OPerlin::Permute(int32_t* P, int32_t N)
{
	for (int i = N - 1; i > 0; i--)
	{
		int target = Utils::Math::Random(0, i);
		int tmp = P[i];
		P[i] = P[target];
		P[target] = tmp;
	}
}

double OPerlin::TrilinearInterpolation(SVec3 c[2][2][2], double U, double V, double W)
{
	// These variables are used to smooth the interpolation.
	// They apply a curve to the interpolation which makes transitions smoother.
	auto uu = U * U * (3 - 2 * U);
	auto vv = V * V * (3 - 2 * V);
	auto ww = W * W * (3 - 2 * W);
	auto accum = 0.0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				// Compute a vector from the corner to the point inside the cell.
				SVec3 weightV{ static_cast<float>(U) - i, static_cast<float>(V) - j, static_cast<float>(W) - k };

				// Accumulate weighted contributions from each corner of the cell.
				accum += (i * uu + (1 - i) * (1 - uu)) * // Weight contribution from the x dimension
					(j * vv + (1 - j) * (1 - vv)) * // Weight contribution from the y dimension
					(k * ww + (1 - k) * (1 - ww)) * // Weight contribution from the z dimension
					Utils::Math::Dot(c[i][j][k], weightV); // Dot product of the corner value and the weight vector
			}
		}
	}
	return accum;
}
