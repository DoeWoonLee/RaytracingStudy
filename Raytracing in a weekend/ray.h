#pragma once

#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const vec3& a, const vec3& b) { A = a; B = b; }
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};

inline float myDrand()
{
	int iRandom = rand() % 10000;
	return (float)iRandom / 10000;
}


inline vec3 random_in_unit_shpere()
{
	vec3 p;
	do {
		p = 2.0f * vec3(myDrand(), myDrand(), myDrand()) - vec3(1.f, 1.f, 1.f);
	} while (p.squared_length() >= 1.0f);

	return p;
}