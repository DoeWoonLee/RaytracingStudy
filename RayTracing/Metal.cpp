#include "stdafx.h"
#include "Metal.h"
#include "MathUtility.h"

CMetal::CMetal(void):
	m_fFuzzy(0.f)
{
}

CMetal::CMetal(const vec3 & vAlbedo):
	m_vAlbedo(vAlbedo), m_fFuzzy(0.f)
{
}

CMetal::CMetal(const vec3 & vAlbedo, const float & fFuzzy):
	m_vAlbedo(vAlbedo), m_fFuzzy(fFuzzy)
{
}
bool CMetal::Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const
{
	float fDiscriminant = 0.f;

	vec3 vOutDir = CMathUtility::Reflect(InRay.GetDirection(), hRec.vNormal, fDiscriminant);

	if (m_fFuzzy > 0.f)
	{
		vOutDir += CMathUtility::RandUnitSphereVector() * m_fFuzzy;
	}

	
	sRec.specularRay.SetOrigin(hRec.vPos);
	sRec.specularRay.SetDirection(vOutDir);
	sRec.vAttenuation = m_vAlbedo;
	sRec.bIsSpecular = true;
	sRec.pPdfPtr = nullptr;

	return true;
	//return (fDiscriminant < 0.f);


}
