#include "stdafx.h"
#include "CosinePdf.h"

// Worldø°º≠ ≥Ó¿Ω
float CosinePdf::Value(const vec3 & vDirection) const
{
	vec3 vDir = vDirection;
	vDir.Normalize();

	float fCosine = vec3::Dot(m_UVW.w(), vDir) ;


	if (fCosine < 0.f)
		return 0.f;
	else
		return fCosine / XM_PI;
}

vec3 CosinePdf::Generate() const
{
	return m_UVW.Local(CMathUtility::RandCosineDirection());
	//return CMathUtility::RandCosineDirection();
}
