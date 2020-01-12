#include "stdafx.h"
#include "DiffuseLight.h"

CDiffuseLight::CDiffuseLight(void) :
	m_vLight(1.f, 1.f, 1.f)
{
}

CDiffuseLight::CDiffuseLight(const vec3 & vLight):
	m_vLight(vLight)
{
}

vec3 CDiffuseLight::Emitted(const vec3 & vPos)
{
	return m_vLight;
}
