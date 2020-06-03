#pragma once

#include "hittable_list.h"
#include "aarect.h"

class box : public hittable
{
public:
	box() {}
	box(const point3& p0, const point3& p1, std::shared_ptr<material> ptr)
		: m_Min(p0), m_Max(p1)
	{
		m_Sides.add(std::make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
		m_Sides.add(std::make_shared<flip_face>(std::make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)));

		m_Sides.add(std::make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
		m_Sides.add(std::make_shared<flip_face>(std::make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)));

		m_Sides.add(std::make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
		m_Sides.add(std::make_shared<flip_face>(std::make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)));
	}

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const override
	{
		return m_Sides.hit(r, t0, t1, rec);
	}

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override 
	{
		output_box = aabb(m_Min, m_Max);
		return true;
	}

private:
	point3 m_Min;
	point3 m_Max;
	hittable_list m_Sides;
};