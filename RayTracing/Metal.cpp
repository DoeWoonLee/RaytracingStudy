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

bool CMetal::Scatter(HitRecord & Record, const CRay& InRay, CRay & OutRay, vec3 & vColor, float& fPdf)const
{
	float fDiscriminant = 0.f;

	vec3 vOutDir = CMathUtility::Reflect(InRay.GetDirection(), Record.vNormal, fDiscriminant);

	if (m_fFuzzy > 0.f)
	{
		vOutDir += CMathUtility::RandUnitSphereVector() * m_fFuzzy;
	}


	OutRay.SetOrigin(Record.vPos);
	OutRay.SetDirection(vOutDir);
	vColor = m_vAlbedo;

	return (fDiscriminant < 0.f);
}
