#include "stdafx.h"
#include "OrthoNormalBase.h"

vec3 OrthoNormalBase::Local(const vec3 & a) const
{

	return a.x *u() + a.y * v() + a.z *w();
}

void OrthoNormalBase::BuildFromW(const vec3 &vNormal)
{
	m_vAxis[2] = vNormal;
	m_vAxis[2].Normalize();

	vec3 a;
	if (fabs(w().x) > 0.9f)
	{
		a = vec3(0.f, 0.f, 1.f);
	}
	else
	{
		a = vec3(1.0f, 0.f, 0.f);
	}
	m_vAxis[1] = vec3::Cross(m_vAxis[2], a);
	m_vAxis[0] = vec3::Cross(m_vAxis[2], m_vAxis[1]);
}
