#include <iostream>
#include <stdio.h>

#include "sphere.h"
#include "hitable_list.h"
#include <float.h>
#include <random>
#include "camera.h"

vec3 color(const ray& r, hitable *world) {
	// Sphere Color

	hit_record rec;

	// Sphere 
	if (world->hit(r, 0.0f, FLT_MAX, rec)) {
		return 0.5f * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else // Sky
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}
float myDrand()
{
	int iRandom = rand() % 10000;
	return (float)iRandom / 10000;
}
int main() {
	
	int nx = 200;
	int ny = 100;
	int ns = 100;



	hitable* list[2];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5f);
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100.f);
	hitable* world = new hitable_list(list, 2);
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
				col += color(r, world);
			}
			
			col /= float(ns);
			int ir = int(255.99f * col[0]);
			int ig = int(255.99f * col[1]);
			int ib = int(255.99f * col[2]);

			fprintf(fp, "%d %d %d\n", ir, ig, ib);
			//std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	fclose(fp);
}