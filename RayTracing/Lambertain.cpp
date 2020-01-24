#include "stdafx.h"
#include "Lambertain.h"
#include "MathUtility.h"
#include "Public.h"
#include "ConstantTexture.h"

CLambertain::CLambertain(void) :
	m_pAlbedo(new ConstantTexture(vec3(0.5f, 0.5f, 0.5f)))
{
	
}

CLambertain::CLambertain(vec3 & vAlbedo)
	:m_pAlbedo(new ConstantTexture(vAlbedo))
{
}

CLambertain::CLambertain(ITexture* pTexture) :
	m_pAlbedo(pTexture)
{
	m_pAlbedo->AddRef();
}

CLambertain::~CLambertain(void)
{
	SAFE_RELEASE(m_pAlbedo);
}

bool CLambertain::Scatter(HitRecord & Record, const CRay& InRay, CRay & OutRay, vec3 & vColor, float& fPdf) const
{
	vColor = m_pAlbedo->Value(0.f,0.f, Record.vPos);

	/*vec3 vDirection;
	do {
		vDirection = CMathUtility::RandUnitSphereVector();
	} while (vec3::Dot(vDirection, Record.vNormal) < 0.f);*/

	OutRay.SetDirection(XMVector3Normalize( Record.vNormal.ToSIMD() + CMathUtility::RandUnitSphereVector().ToSIMD() ));
	//OutRay.SetDirection(vDirection);

	OutRay.SetOrigin(Record.vPos);
	fPdf = vec3::Dot(Record.vNormal, OutRay.GetDirection()) / XM_PI;
	//fPdf = 0.5f / XM_PI;

	return true;
}

float CLambertain::ScatteringPdf(const CRay & inRay, const CRay & outRay, const HitRecord & hitRecord)
{
	float fCosine = vec3::Dot(hitRecord.vNormal, outRay.GetDirection());
	if (fCosine < 0.f)
		return 0.f;
	return fCosine / XM_PI;

}
