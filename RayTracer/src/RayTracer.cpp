#include "library/math.h"
#include "library/color.h"

#include "library/hittable_list.h"
#include "library/sphere.h"
#include "library/material.h"

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

int main()
{
	const auto aspectRatio = 16.0f / 9.0f;
	const int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int samples_per_pixel = 10;
	const int max_depth = 50;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Define Origin and Axies
	Camera camera(point3(0.0f, 0.0f, 0.0f), vec3(4.0f, 0.0f, 0.0f), vec3(0.0f, 9.0f / 4.0f, 0.0f));

	hittable_list world;
	world.add(std::make_shared <sphere>(point3( 0.0f, 0.0f, -1.0f),   0.5f,   std::make_shared<lambertian>(color(0.7f, 0.3f, 0.3f))));
	world.add(std::make_shared <sphere>(point3( 0.0f, -100.5, -1.0f), 100.0f, std::make_shared<lambertian>(color(0.8f, 0.8f, 0.0f))));
	world.add(std::make_shared <sphere>(point3( 1.0f, 0.0f, -1.0f),   0.5f,   std::make_shared<metal>(color(0.8f, 0.6f, 0.2f), 0.3f)));
	world.add(std::make_shared <sphere>(point3(-1.0f, 0.0f, -1.0f),   0.5f,   std::make_shared<metal>(color(0.8f, 0.8f, 0.8f), 1.0f)));


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