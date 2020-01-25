#include "stdafx.h"
#include "Lambertain.h"
#include "MathUtility.h"
#include "Public.h"
#include "ConstantTexture.h"
#include "OrthoNormalBase.h"
#include "CosinePdf.h"

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

bool CLambertain::Scatter(const HitRecord & hRec, const CRay& InRay, ScatterRecord& sRec) const
{
	sRec.bIsSpecular = false;
	sRec.vAttenuation = m_pAlbedo->Value(hRec.vUV.x, hRec.vUV.y, hRec.vPos);
	sRec.pPdfPtr = new CosinePdf(hRec.vNormal);

	return true;
}

float CLambertain::ScatteringPdf(const HitRecord & hRec, const CRay & inRay, const CRay & outRay) const
{
	float fCosine = vec3::Dot(hRec.vNormal, outRay.GetDirection());
	if (fCosine < 0.f)
		return 0.f;
	return fCosine / XM_PI;

}
