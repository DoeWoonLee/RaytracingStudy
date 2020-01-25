#include "stdafx.h"
#include "Dielectric.h"
#include "MathUtility.h"

CDielectric::CDielectric(void):
	m_vAlbedo(1.f, 1.f, 1.f), m_fRefIdx(1.5f)
{
}

CDielectric::CDielectric(vec3 & vAlbedo, float & fRefIdx) :
	m_vAlbedo(vAlbedo), m_fRefIdx(fRefIdx)
{
}



float Schlick(const float& fCosine,const float& fRefIdx)
{
	// 프레넬 효과를 구할때 사용하는 근사 함수
	// fCosine은 노말과 입사광석의 내적값
	// 특정 매질(굴절상수)에 대해서 일정 각도에
	// 대해서 전반사가 일어나는 현상을 구현할때 사용

	float r0 = (1.f - fRefIdx) / (1.f + fRefIdx);
	r0 = r0 * r0;
	return r0 + (1.f - r0) * powf(1.f - fCosine, 5);
}
bool CDielectric::Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const
{
	/*vec3 vOutwardNormal;
	vec3 vScattered;
	float fNiOverNt = 0.f;
	float fCosine = vec3::Dot(InRay.GetDirection(), Record.vNormal);
	float fReflectProb = 0.f;
	// 광선이 매질에서 대기로 나올때
	if (fCosine> 0.f)
	{
		vOutwardNormal = Record.vNormal * (-1.f);
		fNiOverNt = m_fRefIdx;
		fCosine = fCosine * m_fRefIdx;
	}
	// 광선이 매질의 방향으로 들어갈때
	else
	{
		vOutwardNormal = Record.vNormal;
		fNiOverNt = 1.0f / m_fRefIdx;
		fCosine = -fCosine;
	}
	
	// 굴절이 됐을때
	if (CMathUtility::Refract(InRay.GetDirection(), vOutwardNormal, fNiOverNt, vScattered))
	{
		fReflectProb = CMathUtility::Schlick(fCosine, m_fRefIdx);
	}
	// 반사 되었을때
	else
	{
		fReflectProb = 1.0f;
	}

	// 확률 함수에 의해 반사를 결정짓는 이유는 반사와 굴절이
	// 동시에 일어나기 때문이다 (빛이 100퍼센트 반사되지 않음)
	if (CMathUtility::frand0to1() < fReflectProb)
	{
		float fDiscriminant = 0.f;
		OutRay.SetDirection(CMathUtility::Reflect(InRay.GetDirection(), Record.vNormal, fDiscriminant));
	}
	else
	{
		OutRay.SetDirection(vScattered);
		
	}
	OutRay.SetOrigin(Record.vPos);
	vColor = m_vAlbedo;
	*/
	return true;
}

CDielectric::CDielectric(float & fRefIdx) :
	m_vAlbedo(1.f, 1.f, 1.f), m_fRefIdx(fRefIdx)
{
}
