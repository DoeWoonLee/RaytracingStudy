#pragma once

#include "Vector3.h"

class CMathUtility
{
public:
	static UINT& XorShift32(void);
	static float frand0to1(void);
	static vec3 RandUnitSphereVector(void);
	static vec3 Reflect(const vec3& vDir,const vec3& vNormal, float& fDistriminant);
	static bool Refract(const vec3& vRayDir, const vec3& vNormal,const float& fNiOverNt, vec3& vRefracted);
	static float Schlick(const float& fCosine, const float& fRefIdx);
	
	static vec3 NormalByTri(const vec3& v0, const vec3& v1, const vec3& v2);

};