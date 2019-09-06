#pragma once

#include "Vector3.h"

struct HitRecord
{
	vec3 vPos;
	vec3 vNormal;
	vec3 vAlbedo;
	vec3 vEmitted;
	float fTime;
};