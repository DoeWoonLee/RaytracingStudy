#include <iostream>
#include <stdio.h>

#include "sphere.h"
#include "hitable_list.h"
#include <float.h>
#include <random>
#include "camera.h"
#include "metal.h"
#include "lambertian.h"

vec3 color(const ray& r, hitable *world, int depth) {
	// Sphere Color

	hit_record rec;

	// Sphere 
	if (world->hit(r, 0.001f, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0.f, 0.f, 0.f);
		}
	}
	else // Sky
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

int main() {
	
	int nx = 800;
	int ny = 400;
	int ns = 150;



	hitable* list[4];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5f, new lambertain(vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100.f, new lambertain(vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new sphere(vec3(1.f, 0.f, -1.f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new sphere(vec3(-1.f, 0.f, -1.f), 0.5f, new metal(vec3(0.8f, 0.8f, 0.8f), 1.f));
	hitable* world = new hitable_list(list, 4);
	camera cam;
	FILE* fp;
	fopen_s(&fp,"result.ppm", "w");

	fprintf(fp, "P3\n%d %d\n255\n", nx, ny);
	//std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) 
			{
				float u = float(i + myDrand()) / float(nx);
				float v = float(j + myDrand()) / float(ny);

				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0f);
				col += color(r, world,0);
			}
			
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99f * col[0]);
			int ig = int(255.99f * col[1]);
			int ib = int(255.99f * col[2]);

			fprintf(fp, "%d %d %d\n", ir, ig, ib);
			//std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	fclose(fp);
}