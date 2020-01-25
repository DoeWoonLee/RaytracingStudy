#include "stdafx.h"
#include "Isotropic.h"
#include "MathUtility.h"

CIsotropic::CIsotropic(const vec3 & vAlbedo):
	m_vAlbedo(vAlbedo)
{
}

bool CIsotropic::Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const
{
	sRec.vAttenuation = m_vAlbedo;

	sRec.specularRay.SetOrigin(hRec.vPos);
	sRec.specularRay.SetDirection(CMathUtility::RandUnitSphereVector());

	return true;
}
