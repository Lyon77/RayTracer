#pragma once

#include "math.h"

class texture
{
public:
	virtual color value(float u, float v, const point3& p) const = 0;
};

class solid_color : public texture
{
public:
	solid_color() {}
	solid_color(color c) : m_Color(c) {}

	solid_color(float red, float green, float blue)
		: solid_color(color(red, green, blue)) {}

	virtual color value(float u, float v, const point3& p) const override
	{
		return m_Color;
	}

private:
	color m_Color;
};