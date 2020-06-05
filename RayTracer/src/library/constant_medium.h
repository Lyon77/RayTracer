#pragma once

#include "hittable.h"
#include "material.h"

class constant_medium : public hittable
{
public:
	constant_medium(std::shared_ptr<hittable> b, float d, std::shared_ptr<texture> a)
		: m_Boundary(b), m_NegInverseDensity(-1/d)
	{
		m_PhaseFunction = std::make_shared<isotropic>(a);
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		// Print occasional samples when debugging. To enable, set enableDebug true
		const bool enableDebug = false;
		const bool debugging = enableDebug && random_float() < 0.00001;

		hit_record rec1, rec2;

		if (!m_Boundary->hit(r, -INF, INF, rec1))
			return false;

		if (!m_Boundary->hit(r, rec1.t + 0.0001, INF, rec2))
			return false;

		if (debugging)
			std::cerr << "\nt0=" << rec1.t << ", t1=" << rec2.t << '\n';
		
		if (rec1.t < t_min)
			rec1.t = t_min;
		if (rec2.t > t_max)
			rec2.t = t_max;

		if (rec1.t >= rec2.t)
			return false;

		if (rec1.t < 0)
			rec1.t = 0;

		const float ray_length = r.GetDirection().length();
		const float distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
		const float hit_distance = m_NegInverseDensity * log(random_float());

		if (hit_distance > distance_inside_boundary)
			return false;

		rec.t = rec1.t + hit_distance / ray_length;
		rec.p = r.at(rec.t);

		if (debugging)
		{
			std::cerr << "hit_distance = " << hit_distance << '\n'
				<< "rec.t = " << rec.t << '\n'
				<< "rec.p = " << rec.p << '\n';
		}

		rec.normal = vec3(1.0f, 0.0f, 0.0f); // arbitrary
		rec.front_face = true; // arbitrary
		rec.matPtr = m_PhaseFunction;

		return true;
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override
	{
		return m_Boundary->bounding_box(t0, t1, output_box);
	}

private:
	std::shared_ptr<hittable> m_Boundary;
	float m_NegInverseDensity;
	std::shared_ptr<material> m_PhaseFunction;
};