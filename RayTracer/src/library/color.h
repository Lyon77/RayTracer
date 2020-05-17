#pragma once

#ifndef COLOR_H
#define COLOR_H

#include "math.h"
#include "vec3.h"

#include <iostream>

void writeColor(std::ostream& out, color pixelColor, int samples_per_pixel = 1) {
	float r = pixelColor.x();
	float g = pixelColor.y();
	float b = pixelColor.z();

	// Divide by number of samples per pixel. Approximate gamma correction as sqrt
	float scale = 1.0f / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	out << static_cast<int>(255.999f * clamp(r, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(255.999f * clamp(g, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(255.999f * clamp(b, 0.0f, 0.999f)) << '\n';
}


#endif