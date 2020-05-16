#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	sphere(point3 center, float r) : m_Center(center), m_Radius(r) {};

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		vec3 oc = r.GetOrigin() - m_Center;
		float a = r.GetDirection().length_squared();
		float halfB = dot(oc, r.GetDirection());
		float c = oc.length_squared() - m_Radius * m_Radius;
		float discriminant = halfB * halfB - a * c; // + = 2, 0 = 1, - = 0

		if (discriminant > 0) 
		{
			float root = sqrt(discriminant);
			float temp = (-halfB - root) / a;

			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.at(rec.t);
				vec3 outward_normal = (rec.p - m_Center) / m_Radius;
				rec.set_face_normal(r, outward_normal);
				return true;
			}

			temp = (-halfB + root) / a;
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.at(rec.t);
				vec3 outward_normal = (rec.p - m_Center) / m_Radius;
				rec.set_face_normal(r, outward_normal);
				return true;
			}
		}
		return false;
	}

private:
	point3 m_Center;
	float m_Radius;
};


#endif