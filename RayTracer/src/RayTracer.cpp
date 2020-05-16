#include "library/math.h"
#include "library/color.h"

#include "library/hittable_list.h"
#include "library/sphere.h"

#include "Camera.h"

#include <iostream>

color rayColor(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, INF, rec))
		return 0.5 * (rec.normal + color(1.0f, 1.0f, 1.0f));

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

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Define Origin and Axies
	Camera camera(point3(0.0f, 0.0f, 0.0f), vec3(4.0f, 0.0f, 0.0f), vec3(0.0f, 9.0f / 4.0f, 0.0f));

	hittable_list world;
	world.add(std::make_shared <sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f));
	world.add(std::make_shared <sphere>(point3(0.0f, -100.5, -1.0f), 100.0f));


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
				pixelColor += rayColor(r, world);
			}

			writeColor(std::cout, pixelColor, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}