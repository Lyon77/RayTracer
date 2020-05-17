#ifndef CAMERA_H
#define CAMERA_H

#include "library/math.h"

class Camera {
public:
	Camera(point3 lookfrom, point3 lookat, vec3 up, float vfov, 
		float aspect_ratio, float aperture, float focusDis)
	{
		m_Origin = lookfrom;
		m_LensRadius = aperture / 2;

		float theta = degrees_to_radians(vfov);
		float half_height = tan(theta / 2);
		float half_width = aspect_ratio * half_height;

		m_W = unit_vector(lookfrom - lookat);
		m_U = unit_vector(cross(up, m_W));
		m_V = cross(m_W, m_U);

		m_Lower_left_corner = m_Origin - half_width * focusDis * m_U - half_height * focusDis * m_V - focusDis * m_W;

		m_Horizontal = 2 * half_width * focusDis * m_U;
		m_Vertical = 2 * half_height * focusDis * m_V;
	}

	ray get_ray(float u, float v) const {
		vec3 rd = m_LensRadius * random_in_unit_disk();
		vec3 offset = m_U * rd.x() + v * rd.y();

		return ray(m_Origin + offset, m_Lower_left_corner + u * m_Horizontal + v * m_Vertical - m_Origin - offset);
	}

private:
	point3 m_Origin;
	point3 m_Lower_left_corner;
	vec3 m_Horizontal;
	vec3 m_Vertical;
	vec3 m_U, m_V, m_W;
	float m_LensRadius;
};
#endif