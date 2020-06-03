#include "library/math.h"
#include "library/color.h"

#include "library/hittable_list.h"
#include "library/sphere.h"
#include "library/material.h"
#include "library/bvh.h"

#include "Camera.h"

#include <iostream>

color rayColor(const ray& r, const hittable& world, int depth) {
	if (depth <= 0)
		return color(0, 0, 0);

	hit_record rec;
	if (world.hit(r, 0.001f, INF, rec)) {
		ray scattered;
		color attenuation;

		if (rec.matPtr->scatter(r, rec, attenuation, scattered))
			return attenuation * rayColor(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.GetDirection());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

hittable_list scene() {
	// Textures /////////////////////////////////////////////////
	auto checker = std::make_shared<checker_texture>(
		std::make_shared<solid_color>(0.2, 0.3, 0.1),
		std::make_shared<solid_color>(0.9, 0.9, 0.9)
		);

	auto perlin_texture = std::make_shared<noise_texture>(3.0f);

	//auto earth_texture = std::make_shared<image_texture>("res/textures/earthmap.jpg");
	////////////////////////////////////////////////////////////

	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0.0f, -500.5, 0.0f), 500.0f, std::make_shared<lambertian>(perlin_texture)));

	world.add(std::make_shared<moving_sphere>(point3(0.0f, 0.0f, 0.0f), point3(0.0f, 0.2f, 0.0f), 0.0f, 1.0f, 0.5f, std::make_shared<lambertian>(std::make_shared<solid_color>(0.1f, 0.2f, 0.5f))));
	//world.add(std::make_shared<sphere>(point3( 0.0f, 1.2f, 0.0f), 0.5f, std::make_shared<lambertian>(earth_texture)));
	world.add(std::make_shared<sphere>(point3(1.0f, 0.0f, 0.0f), 0.5f, std::make_shared<metal>(color(0.8f, 0.6f, 0.2f), 0.3f)));
	world.add(std::make_shared<sphere>(point3(-1.0f, 0.0f, 0.0f), 0.5f, std::make_shared<dielectric>(color(1.0f), 1.5f)));

	return world;
}

/////////////////////////////////////////////////////////////////////////////////////
// Main /////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main()
{
	const float aspectRatio = 16.0f / 9.0f;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int samples_per_pixel = 10; // To make the edges not pixelated
	const int max_depth = 50; // How many times the ray will bounce

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Define Origin and Axies
	point3 lookfrom(8, 2, 4);
	point3 lookat(0, 0, 0);
	vec3 up(0, 1, 0);
	float aperture = 0.0f;
	float focusDis = (lookfrom - lookat).length();
	
	Camera camera(lookfrom, lookat, up, 20, aspectRatio, aperture, focusDis, 0.0f, 1.0f);

	hittable_list world = scene();

	for (int j = image_height - 1; j >= 0; --j) 
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			// Iterate through each pixel and generate a ray
			color pixelColor(0.0f, 0.0f, 0.0f);

			for (int s = 0; s < samples_per_pixel; s++) 
			{
				float u = float(i + random_float()) / (image_width - 1.0f);
				float v = float(j + random_float()) / (image_height - 1.0f);
				ray r = camera.get_ray(u, v);
				pixelColor += rayColor(r, world, max_depth);
			}

			writeColor(std::cout, pixelColor, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}