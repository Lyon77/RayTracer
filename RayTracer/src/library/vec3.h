#pragma once

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "math.h"

class vec3 {
public:
	vec3() : e {0, 0, 0} {}
	vec3(float val) : e {val, val, val} {}
	vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	inline static vec3 random() 
	{
		return vec3(random_float(), random_float(), random_float());
	}

	inline static vec3 random(float min, float max) 
	{
		return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator-=(const vec3& v)
	{
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}

	vec3& operator*=(const float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const float t) {
		return *this *= 1 / t;
	}

	float length() const {
		return sqrt(length_squared());
	}

	float length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	float e[3];
};

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, float t) {
	return t * v;
}

inline vec3 operator/(vec3 v, float t) {
	return (1 / t) * v;
}

inline float dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}


vec3 reflect(const vec3& v, const vec3& normal)
{
	return v - 2 * dot(v, normal) * normal;
}

vec3 refract(const vec3& uv, const vec3& normal, float factor)
{
	float cos_theta = dot(-uv, normal);
	vec3 para = factor * (uv + cos_theta * normal);
	vec3 perp = -sqrt(1.0 - para.length_squared()) * normal;
	return para + perp;
}

//////////////////////////////////////////////////////////////////
/// Random ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
vec3 random_in_unit_disk()
{
	while (true)
	{
		vec3 p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
		if (p.length_squared() < 1)
			return p;
	}
}

vec3 random_in_unit_sphere() {
	while (true) {
		vec3 p = vec3::random(-1, 1);
		if (p.length_squared() < 1)
			return p;
	}
}

vec3 random_unit_vector()
{
	float a = random_float(0, 2 * PI);
	float z = random_float(-1, 1);
	float r = sqrt(1 - z * z);
	return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_hemisphere(const vec3& nornal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, nornal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}
//////////////////////////////////////////////////////////////////

using point3 = vec3;
using color = vec3;

#endif