#include "stdafx.h"
#include "Sphere.h"


CSphere::CSphere() : 
	m_vCenter(0.f, 0.f, 0.f), m_fRadius(1.f)
{
	MakeMinMax();
}

CSphere::CSphere(const float & fRadius) : 
	m_vCenter(0.f, 0.f, 0.f), m_fRadius(fRadius)
{
	MakeMinMax();
}

CSphere::CSphere(const vec3 & vCenter, const float & fRadius) :
	m_vCenter(vCenter), m_fRadius(fRadius)
{
	MakeMinMax();
}
void CSphere::MakeMinMax()
{
	m_vMin = vec3(-m_fRadius + m_vCenter.x, -m_fRadius + m_vCenter.y, -m_fRadius + m_vCenter.z);
	m_vMax = vec3(m_fRadius + m_vCenter.x, m_fRadius + m_vCenter.y, m_fRadius + m_vCenter.z);
}

thread_local XMVECTOR sCenter;
thread_local XMVECTOR sDir;
thread_local XMVECTOR sOrigin;
thread_local XMVECTOR R;

bool CSphere::Hit(const CRay & InputRay, float & fMin, float & fMax, HitRecord& hitRecord)const
{
	sCenter = m_vCenter.ToSIMD();
	// Sphere Check
	sDir = InputRay.GetSDirection();
	sOrigin = InputRay.GetSOrigin();

	R = sOrigin - sCenter;

	float A = DirectX::XMVector3Dot(sDir, sDir).m128_f32[0];
	float B = DirectX::XMVector3Dot(R, sDir).m128_f32[0];
	float C = DirectX::XMVector3Dot(R, R).m128_f32[0] - m_fRadius * m_fRadius;

	float fDiscriminant = B * B -  A * C;

	if (fDiscriminant > 0.f)
	{
		float T = (-B - sqrtf(fDiscriminant)) / A;
		if (fMin < T && T < fMax)
		{
			hitRecord.fTime = T;
			hitRecord.vPos = InputRay.PointAtParameter(T);
			hitRecord.vNormal.LoadSIMD((hitRecord.vPos.ToSIMD() - m_vCenter.ToSIMD()) / m_fRadius);

			return true;
		}

		T = (-B + sqrtf(fDiscriminant)) / A;

		if (fMin < T && T < fMax)
		{
			hitRecord.fTime = T;
			hitRecord.vPos = InputRay.PointAtParameter(T);
			hitRecord.vNormal.LoadSIMD((hitRecord.vPos.ToSIMD() - m_vCenter.ToSIMD()) / m_fRadius);
			return true;
		}
	}

	return false;
}

