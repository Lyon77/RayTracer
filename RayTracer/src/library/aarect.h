#pragma once

#include "math.h"
#include "hittable.h"

class xy_rect : public hittable
{
public:
	xy_rect() {}

	xy_rect(float x0, float x1, float y0, float y1, float k, std::shared_ptr<material> mat)
		: m_X0(x0), m_X1(x1), m_Y0(y0), m_Y1(y1), m_K(k), m_Material(mat) {}

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const override
	{
		float t = (m_K - r.GetOrigin().z()) / r.GetDirection().z();

		if (t < t0 || t > t1)
			return false;

		float x = r.GetOrigin().x() + t * r.GetDirection().x();
		float y = r.GetOrigin().y() + t * r.GetDirection().y();

		if (x < m_X0 || x > m_X1 || y < m_Y0 || y > m_Y1)
			return false;

		rec.u = (x - m_X0) / (m_X1 - m_X0);
		rec.v = (y - m_Y0) / (m_Y1 - m_Y0);
		rec.t = t;
		
		vec3 outward_normal = vec3(0.0f, 0.0f, 1.0f);
		rec.set_face_normal(r, outward_normal);
		rec.matPtr = m_Material;
		rec.p = r.at(t);

		return true;
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		output_box = aabb(point3(m_X0, m_Y0, m_K - 0.0001), point3(m_X1, m_Y1, m_K + 0.0001));
		return true;
	}

private:
	std::shared_ptr<material> m_Material;
	float m_X0, m_X1, m_Y0, m_Y1, m_K;
};

class xz_rect : public hittable
{
public:
	xz_rect() {}

	xz_rect(float x0, float x1, float z0, float z1, float k, std::shared_ptr<material> mat)
		: m_X0(x0), m_X1(x1), m_Z0(z0), m_Z1(z1), m_K(k), m_Material(mat) {}

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const override
	{
		float t = (m_K - r.GetOrigin().y()) / r.GetDirection().y();

		if (t < t0 || t > t1)
			return false;

		float x = r.GetOrigin().x() + t * r.GetDirection().x();
		float z = r.GetOrigin().z() + t * r.GetDirection().z();

		if (x < m_X0 || x > m_X1 || z < m_Z0 || z > m_Z1)
			return false;

		rec.u = (x - m_X0) / (m_X1 - m_X0);
		rec.v = (z - m_Z0) / (m_Z1 - m_Z0);
		rec.t = t;

		vec3 outward_normal = vec3(0.0f, 1.0f, 0.0f);
		rec.set_face_normal(r, outward_normal);
		rec.matPtr = m_Material;
		rec.p = r.at(t);

		return true;
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		output_box = aabb(point3(m_X0, m_K - 0.0001, m_Z0), point3(m_X1, m_K + 0.0001, m_Z1));
		return true;
	}

private:
	std::shared_ptr<material> m_Material;
	float m_X0, m_X1, m_Z0, m_Z1, m_K;
};

class yz_rect : public hittable
{
public:
	yz_rect() {}

	yz_rect(float y0, float y1, float z0, float z1, float k, std::shared_ptr<material> mat)
		: m_Z0(z0), m_Z1(z1), m_Y0(y0), m_Y1(y1), m_K(k), m_Material(mat) {}

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const override
	{
		float t = (m_K - r.GetOrigin().x()) / r.GetDirection().x();

		if (t < t0 || t > t1)
			return false;

		float z = r.GetOrigin().z() + t * r.GetDirection().z();
		float y = r.GetOrigin().y() + t * r.GetDirection().y();

		if (z < m_Z0 || z > m_Z1 || y < m_Y0 || y > m_Y1)
			return false;

		rec.u = (y - m_Y0) / (m_Y1 - m_Y0);
		rec.v = (z - m_Z0) / (m_Z1 - m_Z0);
		rec.t = t;

		vec3 outward_normal = vec3(1.0f, 0.0f, 0.0f);
		rec.set_face_normal(r, outward_normal);
		rec.matPtr = m_Material;
		rec.p = r.at(t);

		return true;
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		output_box = aabb(point3(m_K - 0.0001, m_Y0, m_Z0), point3(m_K + 0.0001, m_Y1, m_Z1));
		return true;
	}

private:
	std::shared_ptr<material> m_Material;
	float m_Z0, m_Z1, m_Y0, m_Y1, m_K;
};