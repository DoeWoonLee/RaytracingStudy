#pragma once

#include "Vector3.h"
#include "Ray.h"

class CCamera
{
public:
	explicit CCamera(vec3 vEye, vec3 vAt, vec3 vUp, float fAspect, float fFov);

	CRay GetRay(float& u, float& v);
private:
	vec3 m_vEye;

	vec3 m_vLowerLeftCorner;
	vec3 m_vHorizontal;
	vec3 m_vVertical;
};