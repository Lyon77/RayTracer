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

class checker_texture : public texture
{
public:
	checker_texture() {}
	checker_texture(std::shared_ptr<texture> t0, std::shared_ptr<texture> t1)
		: m_Even(t0), m_Odd(t1) {}

	virtual color value(float u, float v, const point3& p) const override
	{
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return m_Odd->value(u, v, p);
		else
			return m_Even->value(u, v, p);
	}

private:
	std::shared_ptr<texture> m_Even, m_Odd;
};