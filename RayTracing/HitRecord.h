#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "Ray.h"
#include "Pdf.h"

struct HitRecord
{
	vec3 vPos;
	vec3 vNormal;
	vec3 vAlbedo;
	vec3 vEmitted;
	vec2 vUV;
	float fTime;

	CRay localRay;
};
struct ScatterRecord
{
	CRay specularRay;
	bool bIsSpecular;
	vec3 vAttenuation;
	IPdf* pPdfPtr;
};