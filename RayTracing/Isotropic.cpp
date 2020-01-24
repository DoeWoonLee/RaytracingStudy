#include "stdafx.h"
#include "Isotropic.h"
#include "MathUtility.h"

CIsotropic::CIsotropic(const vec3 & vAlbedo):
	m_vAlbedo(vAlbedo)
{
}

bool CIsotropic::Scatter(HitRecord & Record, const CRay & InRay, CRay & OutRay, vec3 & vColor, float& fPdf)const
{
	vColor = m_vAlbedo;

	OutRay.SetOrigin(Record.vPos);
	OutRay.SetDirection(CMathUtility::RandUnitSphereVector());

	return true;
}
