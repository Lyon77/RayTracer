#pragma once

#include "math.h"

inline float trilinear_interp(vec3 c[2][2][2], float u, float v, float w)
{
	auto uu = u * u * (3 - 2 * u);
	auto vv = v * v * (3 - 2 * v);
	auto ww = w * w * (3 - 2 * w);
	float accum = 0.0f;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				vec3 weight(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu)) *
					     (j * vv + (1 - j) * (1 - vv)) *
					     (k * ww + (1 - k) * (1 - ww)) * 
					     dot(c[i][j][k], weight);
			}
	return accum;
}

class perlin
{
public:
	perlin()
	{
		m_Ranvec = new vec3[s_PointCount];
		for (int i = 0; i < s_PointCount; ++i)
		{
			m_Ranvec[i] = unit_vector(vec3::random(-1, 1));
		}
		
		m_PermX = perlin_generate_perm();
		m_PermY = perlin_generate_perm();
		m_PermZ = perlin_generate_perm();
	}

	~perlin()
	{
		delete[] m_Ranvec;
		delete[] m_PermX;
		delete[] m_PermY;
		delete[] m_PermZ;
	}

	float noise(const point3& p) const
	{
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());

		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = m_Ranvec[
						m_PermX[(i + di) & 255] ^
						m_PermY[(j + dj) & 255] ^
						m_PermZ[(k + dk) & 255]
					];

		return trilinear_interp(c, u, v, w);
	}

	float turbulence(const point3& p, int depth = 7) const
	{
		float accum = 0.0f;
		point3 temp_p = p;
		float weight = 1.0f;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(temp_p);
			weight *= 0.5f;
			temp_p *= 2.0f;
		}

		return fabs(accum);
	}

private:
	static const int s_PointCount = 256;

	vec3* m_Ranvec;
	int* m_PermX;
	int* m_PermY;
	int* m_PermZ;

	static int* perlin_generate_perm()
	{
		auto p = new int[s_PointCount];

		for (int i = 0; i < perlin::s_PointCount; i++)
			p[i] = i;

		permute(p, s_PointCount);

		return p;
	}

	static void permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int target = random_int(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}
};