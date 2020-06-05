#include "library/math.h"
#include "library/color.h"

#include "library/hittable_list.h"
#include "library/sphere.h"
#include "library/material.h"
#include "library/bvh.h"
#include "library/aarect.h"
#include "library/box.h"
#include "library/constant_medium.h"

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

hittable_list scene() 
{
	hittable_list objects;

	// Ground
	hittable_list boxes1;
	auto ground = std::make_shared<lambertian>(std::make_shared<solid_color>(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_float(1, 101);
			auto z1 = z0 + w;

			boxes1.add(std::make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}	
	objects.add(std::make_shared<bvh_node>(boxes1, 0, 1));

	// Light
	auto light = std::make_shared<diffuse_light>(std::make_shared<solid_color>(7, 7, 7));
	objects.add(std::make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	
	// Moving Sphere
	auto center1 = point3(400, 400, 200);
	auto center2 = center1 + vec3(20, 0, 0);
	auto moving_sphere_material =
		std::make_shared<lambertian>(std::make_shared<solid_color>(0.7, 0.3, 0.1));
	objects.add(std::make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	// Metal and Dielectric spheres
	objects.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(color(1), 1.5)));
	objects.add(std::make_shared<sphere>(
		point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 10.0)
		));

	// Fog Spheres
	auto boundary = std::make_shared<sphere>(point3(360, 150, 145), 70, std::make_shared<dielectric>(color(1), 1.5));
	objects.add(boundary);
	objects.add(std::make_shared<constant_medium>(
		boundary, 0.2, std::make_shared<solid_color>(0.2, 0.4, 0.9)
		));
	boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(color(1), 1.5));
	objects.add(std::make_shared<constant_medium>(
		boundary, .0001, std::make_shared<solid_color>(1, 1, 1)));

	// Noise Sphere
	auto pertext = std::make_shared<noise_texture>(0.1);
	objects.add(std::make_shared<sphere>(point3(220, 280, 300), 80, std::make_shared<lambertian>(pertext)));
	

	// Box made of Spheres
	hittable_list boxes2;
	auto white = std::make_shared<lambertian>(std::make_shared<solid_color>(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(std::make_shared<sphere>(point3::random(0, 165), 10, white));
	}

	objects.add(std::make_shared<translate>(
		std::make_shared<rotate_y>(std::make_shared<bvh_node>(boxes2, 0.0, 1.0), 15), vec3(-100, 270, 395))
	);

	return objects;
}

hittable_list cornell_box() {
	hittable_list objects;

	auto red = std::make_shared<lambertian>(std::make_shared<solid_color>(.65, .05, .05));
	auto white = std::make_shared<lambertian>(std::make_shared<solid_color>(.73, .73, .73));
	auto green = std::make_shared<lambertian>(std::make_shared<solid_color>(.12, .45, .15));
	auto light = std::make_shared<diffuse_light>(std::make_shared<solid_color>(7, 7, 7));

	// Room
	objects.add(std::make_shared<flip_face>(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
	objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(std::make_shared<xz_rect>(113, 443, 127, 432, 554, light));
	objects.add(std::make_shared<flip_face>(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white)));
	objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(std::make_shared<flip_face>(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

	// Inside
	std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = std::make_shared<rotate_y>(box1, 15);
	box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));

	std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = std::make_shared<rotate_y>(box2, -18);
	box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));

	objects.add(std::make_shared<constant_medium>(box1, 0.01, std::make_shared<solid_color>(0, 0, 0)));
	objects.add(std::make_shared<constant_medium>(box2, 0.01, std::make_shared<solid_color>(1, 1, 1)));

	return objects;
}

/////////////////////////////////////////////////////////////////////////////////////
// Main /////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main()
{
	const float aspectRatio = 9.0f / 9.0f;
	const int image_width = 600;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int samples_per_pixel = 300; // To make the edges not pixelated
	const int max_depth = 50; // How many times the ray will bounce

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	point3 lookfrom(478, 278, -600);
	point3 lookat(278, 278, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;
	auto vfov = 40.0;

	Camera camera(lookfrom, lookat, vup, vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);

	const color background(0, 0, 0);
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
				pixelColor += rayColor(r, background, world, max_depth);
			}

			writeColor(std::cout, pixelColor, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}