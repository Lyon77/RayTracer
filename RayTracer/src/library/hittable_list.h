#pragma once

#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:
	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void clear() { m_Objects.clear(); }
	void add(std::shared_ptr<hittable> object) { m_Objects.push_back(object); }

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		hit_record temp_rec;
		bool hit_anything = false;
		float closest_so_far = t_max;

		for (const auto& object : m_Objects)
		{
			if (object->hit(r, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}

private:
	std::vector<std::shared_ptr<hittable>> m_Objects;
};

#endif