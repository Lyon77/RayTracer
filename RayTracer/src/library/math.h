#pragma once

#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <functional>
#include <random>
#include <limits>
#include <memory>

// Constants

const double INF = std::numeric_limits<float>::infinity();
const double PI = 3.1415926535897932385;

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
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<float()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

inline float random_float(float min, float max) 
{
	return min + (max - min) * random_float();
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif