#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	sphere() {}
	sphere(point3 center, float r, std::shared_ptr<material> m)
		: m_Center(center), m_Radius(r), m_MatPtr(m) {};

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
				rec.matPtr = m_MatPtr;
				return true;
			}

			temp = (-halfB + root) / a;
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.at(rec.t);
				vec3 outward_normal = (rec.p - m_Center) / m_Radius;
				rec.set_face_normal(r, outward_normal);
				rec.matPtr = m_MatPtr;
				return true;
			}
		}
		return false;
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override {
		output_box = aabb(m_Center - vec3(m_Radius, m_Radius, m_Radius), m_Center + vec3(m_Radius, m_Radius, m_Radius));
		return true;
	}

private:
	point3 m_Center;
	float m_Radius;
	std::shared_ptr<material> m_MatPtr;
};

class moving_sphere : public hittable
{
public:
	moving_sphere() {}
	moving_sphere(point3 center0, point3 center1, float t0, float t1, float r, std::shared_ptr<material> m)
		: m_Center0(center0), m_Center1(center1), m_T0(t0), m_T1(t1), m_Radius(r), m_MatPtr(m) {
	};

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		vec3 oc = r.GetOrigin() - GetCenter(r.GetTime());
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
				vec3 outward_normal = (rec.p - GetCenter(r.GetTime())) / m_Radius;
				rec.set_face_normal(r, outward_normal);
				rec.matPtr = m_MatPtr;
				return true;
			}

			temp = (-halfB + root) / a;
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.at(rec.t);
				vec3 outward_normal = (rec.p - GetCenter(r.GetTime())) / m_Radius;
				rec.set_face_normal(r, outward_normal);
				rec.matPtr = m_MatPtr;
				return true;
			}
		}
		return false;
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override {
		aabb box0 = aabb(m_Center0 - vec3(m_Radius, m_Radius, m_Radius), m_Center0 + vec3(m_Radius, m_Radius, m_Radius));
		aabb box1 = aabb(m_Center1 - vec3(m_Radius, m_Radius, m_Radius), m_Center1 + vec3(m_Radius, m_Radius, m_Radius));

		output_box = surrounding_box(box0, box1);
		return true;
	}

	point3 GetCenter(float time) const {
		return m_Center0 + ((time - m_T0) / (m_T1 - m_T0)) * (m_Center1 - m_Center0);
	}

private:
	point3 m_Center0, m_Center1;
	float m_T0, m_T1;
	float m_Radius;
	std::shared_ptr<material> m_MatPtr;
};


#endif