#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

class material
{
public:
	virtual color emitted(float u, float v, const point3& p) const
	{
		return color(0.0f);
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(std::shared_ptr<texture> a)
		: m_Albedo(a) {};

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 scatterDir = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatterDir, r_in.GetTime());
		attenuation = m_Albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
private:
	std::shared_ptr<texture> m_Albedo;
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

// Determines whether to reflect depending on the incoming angle
float schlick(float cosine, float index)
{
	float r0 = (1 - index) / (1 + index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class dielectric : public material
{
public:
	dielectric(const color& a, float index)
		: m_Albedo(a), m_Idx(index) {};

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = m_Albedo;

		float factor;
		if (rec.front_face)
			factor = 1.0f / m_Idx;
		else
			factor = m_Idx;

		vec3 unit_direction = unit_vector(r_in.GetDirection());
		float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
		float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

		float reflectProb = schlick(cos_theta, factor);

		// Reflect if the factor is too large
		if (factor * sin_theta > 1.0f || random_float() < reflectProb)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		vec3 refracted = refract(unit_direction, rec.normal, factor);
		scattered = ray(rec.p, refracted);
		return true;
	}
private:
	color m_Albedo;
	float m_Idx;
};

class diffuse_light : public material
{
public:
	diffuse_light(std::shared_ptr<texture> a)
		: m_Emit(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		return false;
	}

	virtual color emitted(float u, float v, const point3& p) const override
	{
		return m_Emit->value(u, v, p);
	}

private:
	std::shared_ptr<texture> m_Emit;
};

#endif