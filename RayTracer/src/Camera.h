#ifndef CAMERA_H
#define CAMERA_H

#include "library/math.h"

class Camera {
public:
	Camera(point3 origin = point3(0.0f, 0.0f, 0.0f), vec3 horizontal = vec3(4.0f, 0.0f, 0.0f), vec3 vertical = vec3(0.0f, 9.0f / 4.0f, 0.0f))
		: m_Origin(origin), m_Horizontal(horizontal), m_Vertical(vertical)
	{
		m_Lower_left_corner = m_Origin - m_Horizontal / 2 - m_Vertical / 2 - vec3(0.0f, 0.0f, 1.0f);
	}

	ray get_ray(float u, float v) const {
		return ray(m_Origin, m_Lower_left_corner + u * m_Horizontal + v * m_Vertical - m_Origin);
	}

private:
	point3 m_Origin;
	point3 m_Lower_left_corner;
	vec3 m_Horizontal;
	vec3 m_Vertical;
};
#endif