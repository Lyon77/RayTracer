#pragma once

#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include <iostream>

class ray {
public:
	ray() {}
	ray(const point3& origin, const vec3& direction)
		: m_Origin(origin), m_Direction(direction) {}

	point3 GetOrigin() const { return m_Origin; }
	vec3 GetDirection() const { return m_Direction; }

	point3 at(float t) const { return m_Origin + t * m_Direction; }

private:
	point3 m_Origin;
	vec3 m_Direction;
};


#endif