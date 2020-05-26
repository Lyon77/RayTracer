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

#endif