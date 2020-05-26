#pragma once

#include "math.h"

class aabb
{
public:
	aabb() {}
	aabb(const point3& a, const point3& b)
		: m_Min(a), m_Max(b) {}

	point3 GetMin() { return m_Min; }
	point3 GetMax() { return m_Max; }

	bool hit(const ray& r, float tmin, float tmax) const
	{
		for (int a = 0; a < 3; a++)
		{
			float invD = 1.0f / r.GetDirection()[a];
			float t0 = (m_Min[a] - r.GetOrigin()[a]) * invD;
			float t1 = (m_Max[a] - r.GetOrigin()[a]) * invD;

			if (invD < 0.0f)
				std::swap(t0, t1);

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if (tmax <= tmin)
				return false;
		}
		return true;
	}
private:
	point3 m_Min, m_Max;
};

aabb surrounding_box(aabb box0, aabb box1)
{
	point3 small(fmin(box0.GetMin().x(), box1.GetMin().x()),
		         fmin(box0.GetMin().y(), box1.GetMin().y()),
		         fmin(box0.GetMin().z(), box1.GetMin().z()));
	point3 big(fmin(box0.GetMax().x(), box1.GetMax().x()),
		       fmin(box0.GetMax().y(), box1.GetMax().y()),
		       fmin(box0.GetMax().z(), box1.GetMax().z()));

	return aabb(small, big);
}