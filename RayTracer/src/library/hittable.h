#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "math.h"
#include "ray.h"
#include "aabb.h"

class material; // to notify hittables of materials

struct hit_record {
	point3 p;
	vec3 normal;
	std::shared_ptr<material> matPtr;
	float t;
	float u, v; // Surface texture Coords
	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.GetDirection(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& output_box) const = 0;
};

class flip_face : public hittable
{
public:
	flip_face(std::shared_ptr<hittable> ptr)
		: m_Ptr(ptr) {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		if (!m_Ptr->hit(r, t_min, t_max, rec))
			return false;

		rec.front_face = !rec.front_face;
		return true;
	};

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override
	{
		return m_Ptr->bounding_box(t0, t1, output_box);
	};

private:
	std::shared_ptr<hittable> m_Ptr;
};

class translate : public hittable
{
public:
	translate(std::shared_ptr<hittable> p, const vec3& displacement)
		: m_Ptr(p), m_Offset(displacement) {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		ray movedRay(r.GetOrigin() - m_Offset, r.GetDirection(), r.GetTime());

		if (!m_Ptr->hit(movedRay, t_min, t_max, rec))
			return false;

		rec.p += m_Offset;
		rec.set_face_normal(movedRay, rec.normal);

		return true;
	};

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override
	{
		if (!m_Ptr->bounding_box(t0, t1, output_box))
			return false;

		output_box = aabb(output_box.GetMin() + m_Offset, output_box.GetMax() + m_Offset);
		return true;
	};

private:
	std::shared_ptr<hittable> m_Ptr;
	vec3 m_Offset;
};

class rotate_y : public hittable
{
public:
	rotate_y(std::shared_ptr<hittable> p, float angle)
		: m_Ptr(p) 
	{
		float radians = degrees_to_radians(angle);
		sin_theta = sin(radians);
		cos_theta = cos(radians);
		m_HasBox = m_Ptr->bounding_box(0, 1, m_BB);

		point3 min(INF);
		point3 max(-INF);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				float x = i * m_BB.GetMax().x() + (1 - i) * m_BB.GetMin().x();
				float y = i * m_BB.GetMax().y() + (1 - i) * m_BB.GetMin().y();
				float z = i * m_BB.GetMax().z() + (1 - i) * m_BB.GetMin().z();

				float newX = cos_theta * x + sin_theta * z;
				float newZ = -sin_theta * x + cos_theta * z;

				vec3 tester(newX, y, newZ);

				for (int c = 0; c < 3; c++)
				{
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmin(max[c], tester[c]);
				}
			}
		}

		m_BB = aabb(min, max);
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		vec3 origin = r.GetOrigin();
		vec3 direction = r.GetDirection();

		origin[0] = cos_theta * r.GetOrigin()[0] - sin_theta * r.GetOrigin()[2];
		origin[2] = sin_theta * r.GetOrigin()[0] + cos_theta * r.GetOrigin()[2];

		direction[0] = cos_theta * r.GetDirection()[0] - sin_theta * r.GetDirection()[2];
		direction[2] = sin_theta * r.GetDirection()[0] + cos_theta * r.GetDirection()[2];

		ray rotated_r(origin, direction, r.GetTime());

		if (!m_Ptr->hit(rotated_r, t_min, t_max, rec))
			return false;

		vec3 p = rec.p;
		vec3 normal = rec.normal;

		p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
		p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

		normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
		normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

		rec.p = p;
		rec.set_face_normal(rotated_r, normal);

		return true;
	};

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override
	{
		output_box = m_BB;
		return m_HasBox;
	};

private:
	std::shared_ptr<hittable> m_Ptr;
	float sin_theta;
	float cos_theta;
	bool m_HasBox;
	aabb m_BB;
};

#endif