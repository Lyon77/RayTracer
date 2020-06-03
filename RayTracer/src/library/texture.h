#pragma once

#include "math.h"
#include "noise.h"

#include "stb_image.h"
#include <iostream>

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

class noise_texture : public texture
{
public:
	noise_texture() : m_Scale(1.0f) {}
	noise_texture(float scale) : m_Scale(scale) {}

	virtual color value(float u, float v, const point3& p) const override
	{
		return color(1, 1, 1) * 0.5 * (1.0 + sin(m_Scale * p.z() + 10 * m_Noise.turbulence(m_Scale * p))); // Prevent negative numbers from noise
		//return color(1, 1, 1) * 0.5 * (1.0 + m_Noise.noise(m_Scale * p)); // Prevent negative numbers from noise
	}

private:
	perlin m_Noise;
	float m_Scale;
};

class image_texture : public texture
{
public:
	const static int s_BytesPerPixel = 3;

	image_texture() 
		: m_Data(nullptr), m_Width(0), m_Height(0), m_BytesPerLine(0) {}

	image_texture(const char* filename)
	{
		auto components_per_pixel = s_BytesPerPixel;

		m_Data = stbi_load(filename, &m_Width, &m_Height, &components_per_pixel, components_per_pixel);

		if (!m_Data)
		{
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
			m_Width = m_Height = 0;
		}
		
		m_BytesPerLine = s_BytesPerPixel * m_Width;
	}

	~image_texture()
	{
		delete m_Data;
	}

	virtual color value(float u, float v, const point3& p) const override
	{
		// If we have no texture data, then return solid cyan as a debugging aid.
		if (m_Data == nullptr)
			return color(0, 1, 1);
		
		// Clamp input texture coordinates to [0,1] x [1,0]
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

		auto i = static_cast<int>(u * m_Width);
		auto j = static_cast<int>(v * m_Height);

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= m_Width)  i = m_Width - 1;
		if (j >= m_Height) j = m_Height - 1;

		const auto color_scale = 1.0 / 255.0;
		auto pixel = m_Data + j * m_BytesPerLine + i * s_BytesPerPixel;

		std::cerr << m_Data << " " << j << " " << "\n";
		return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}

private:
	unsigned char* m_Data;
	int m_Width, m_Height;
	int m_BytesPerLine;
};