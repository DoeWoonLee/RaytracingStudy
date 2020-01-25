#include "stdafx.h"
#include "OrthoNormalBase.h"

void OrthoNormalBase::BuildFromW(const vec3 &vNormal)
{
	m_vAxis[2] = vNormal;
	m_vAxis[2].Normalize();

	vec3 a;
	if (fabs(w().x) > 0.9f)
	{
		a = vec3(0.f, 1.f, 0.f);
	}
	else
	{
		a = vec3(1.0f, 0.f, 0.f);
	}
	m_vAxis[1] = vec3::Cross(w(), a);
	m_vAxis[0] = vec3::Cross(w(), v());
}
