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

	// Divide by number of samples per pixel
	float scale = 1.0f / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}


#endif