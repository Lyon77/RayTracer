#pragma once

#include "hittable_list.h"

#include <algorithm>

inline bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis) {
	aabb box_a;
	aabb box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.GetMin().e[axis] < box_b.GetMin().e[axis];
}

bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

class bvh_node : public hittable
{
public:
	bvh_node() {}
	bvh_node(hittable_list& list, float time0, float time1)
		: bvh_node(list.m_Objects, 0, list.GetObjects().size(), time0, time1)
	{}

	// Randomly choose an axis, sort it, and then put half in each subtree
	bvh_node(std::vector<std::shared_ptr<hittable>>& objects,
		size_t start, size_t end, double time0, double time1)
	{
		int axis = random_int(0, 2);
		auto comparator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare : box_z_compare;

		size_t object_span = end - start;

		if (object_span == 1)
			m_Left = m_Right = objects[start];
		else if (object_span == 2)
		{
			if (comparator(objects[start], objects[start + 1]))
			{
				m_Left = objects[start];
				m_Right = objects[start + 1];
			}
			else
			{
				m_Left = objects[start + 1];
				m_Right = objects[start];
			}
		}
		else
		{
			std::sort(objects.begin() + start, objects.begin() + end, comparator);
			auto mid = start + object_span / 2;
			m_Left = std::make_shared<bvh_node>(objects, start, mid, time0, time1);
			m_Right = std::make_shared<bvh_node>(objects, mid, end, time0, time1);
		}

		aabb box_left, box_right;

		if (!m_Left->bounding_box(time0, time1, box_left) || !m_Right->bounding_box(time0, time1, box_right))
			std::cerr << "No bounding box in bvh_node constructor.\n";

		m_Box = surrounding_box(box_left, box_right);
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		if (!m_Box.hit(r, t_min, t_max))
			return false;

		bool hit_left = m_Left->hit(r, t_min, t_max, rec);
		bool hit_right = m_Right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

		return hit_left || hit_right;
	};

	virtual bool bounding_box(float t0, float t1, aabb& output_box) const override
	{
		output_box = m_Box;
		return true;
	};

private:
	std::shared_ptr<hittable> m_Left;
	std::shared_ptr<hittable> m_Right;
	aabb m_Box;
};

