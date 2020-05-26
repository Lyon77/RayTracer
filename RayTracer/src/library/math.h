#pragma once

#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <functional>
#include <random>
#include <limits>
#include <memory>

// Constants

const float INF = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385f;

// Utility Functions
inline double degrees_to_radians(float degrees) {
	return degrees * PI / 180;
}

inline float clamp(float x, float min, float max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

inline float random_float() 
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<float()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

inline float random_float(float min, float max) 
{
	return min + (max - min) * random_float();
}

inline int random_int(int min, int max)
{
	static std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);
	static std::mt19937 generator;
	static std::function<int()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif