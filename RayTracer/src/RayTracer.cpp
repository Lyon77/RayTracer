#include "library/math.h"
#include "library/color.h"

#include "library/hittable_list.h"
#include "library/sphere.h"
#include "library/material.h"
#include "library/bvh.h"
#include "library/aarect.h"
#include "library/box.h"

#include "Camera.h"

#include <iostream>

color rayColor(const ray& r, const color& background, const hittable& world, int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	hit_record rec;

	// If the ray hits nothing, return the background color.
	if (!world.hit(r, 0.001f, INF, rec))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

	if (!rec.matPtr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * rayColor(scattered, background, world, depth - 1);
}

hittable_list scene() {
	// Textures /////////////////////////////////////////////////
	auto checker = std::make_shared<checker_texture>(
		std::make_shared<solid_color>(0.2, 0.3, 0.1),
		std::make_shared<solid_color>(0.9, 0.9, 0.9)
		);

	auto perlin_texture = std::make_shared<noise_texture>(3.0f);
	auto difflight = std::make_shared<diffuse_light>(std::make_shared<solid_color>(2.0f, 2.0f, 2.0f));

	//auto earth_texture = std::make_shared<image_texture>("res/textures/earthmap.jpg");
	////////////////////////////////////////////////////////////

	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0.0f, -500 - 1, 0.0f), 500.0f, std::make_shared<lambertian>(perlin_texture)));

	// Objects
	//world.add(std::make_shared<moving_sphere>(point3(0.0f, 0.0f, 0.0f), point3(0.0f, 0.2f, 0.0f), 0.0f, 1.0f, 1.0f, std::make_shared<lambertian>(std::make_shared<solid_color>(0.1f, 0.2f, 0.5f))));
	//world.add(std::make_shared<sphere>(point3( 0.0f, 2.2f, 0.0f), 1.0f, std::make_shared<lambertian>(earth_texture)));
	world.add(std::make_shared<sphere>(point3(2.0f, 0.0f, 0.0f), 1.0f, std::make_shared<metal>(color(0.8f, 0.6f, 0.2f), 0.3f)));
	//world.add(std::make_shared<sphere>(point3(-2.0f, 0.0f, 0.0f), 1.0f, std::make_shared<dielectric>(color(1.0f), 1.5f)));

	// Lights
	world.add(std::make_shared<xy_rect>(-0.5f, 0.5f, 0, 0.5f, -1, difflight));
	world.add(std::make_shared<sphere>(point3(0.0f, 2.0f, 1.0f), 0.5f, difflight));

	return world;
}

hittable_list cornell_box() {
	hittable_list objects;

	auto red = std::make_shared<lambertian>(std::make_shared<solid_color>(.65, .05, .05));
	auto white = std::make_shared<lambertian>(std::make_shared<solid_color>(.73, .73, .73));
	auto green = std::make_shared<lambertian>(std::make_shared<solid_color>(.12, .45, .15));
	auto light = std::make_shared<diffuse_light>(std::make_shared<solid_color>(15, 15, 15));

	// Room
	objects.add(std::make_shared<flip_face>(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
	objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(std::make_shared<flip_face>(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white)));
	objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(std::make_shared<flip_face>(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

	// Inside
	objects.add(std::make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
	objects.add(std::make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));

	return objects;
}

/////////////////////////////////////////////////////////////////////////////////////
// Main /////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main()
{
	const float aspectRatio = 9.0f / 9.0f;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int samples_per_pixel = 40; // To make the edges not pixelated
	const int max_depth = 50; // How many times the ray will bounce

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Define Origin and Axies
	/*point3 lookfrom(10, 10, 8);
	point3 lookat(0, 0, 0);
	vec3 up(0, 1, 0);
	float aperture = 0.0f;
	float focusDis = (lookfrom - lookat).length();
	
	Camera camera(lookfrom, lookat, up, 20, aspectRatio, aperture, focusDis, 0.0f, 1.0f);*/

	point3 lookfrom(278, 278, -800);
	point3 lookat(278, 278, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;
	auto vfov = 40.0;

	Camera camera(lookfrom, lookat, vup, vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);

	const color background(0, 0, 0);
	hittable_list world = cornell_box();

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
				pixelColor += rayColor(r, background, world, max_depth);
			}

			writeColor(std::cout, pixelColor, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}