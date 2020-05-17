#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const color& a)
		: m_Albedo(a) {};

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 scatterDir = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatterDir);
		attenuation = m_Albedo;
		return true;
	}
private:
	color m_Albedo;
};

class metal : public material
{
public:
	metal(const color& a, float f)
		: m_Albedo(a), m_Fuzz(f < 1 ? f : 1) {};

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 reflected = reflect(unit_vector(r_in.GetDirection()), rec.normal);
		scattered = ray(rec.p, reflected + m_Fuzz * random_in_unit_sphere());
		attenuation = m_Albedo;
		return (dot(scattered.GetDirection(), rec.normal) > 0);
	}
private:
	color m_Albedo;
	float m_Fuzz;
};

#endif