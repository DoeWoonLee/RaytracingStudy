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
	// ������ ȿ���� ���Ҷ� ����ϴ� �ٻ� �Լ�
	// fCosine�� �븻�� �Ի籤���� ������
	// Ư�� ����(�������)�� ���ؼ� ���� ������
	// ���ؼ� ���ݻ簡 �Ͼ�� ������ �����Ҷ� ���

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
	// ������ �������� ���� ���ö�
	if (fCosine> 0.f)
	{
		vOutwardNormal = Record.vNormal * (-1.f);
		fNiOverNt = m_fRefIdx;
		fCosine = fCosine * m_fRefIdx;
	}
	// ������ ������ �������� ����
	else
	{
		vOutwardNormal = Record.vNormal;
		fNiOverNt = 1.0f / m_fRefIdx;
		fCosine = -fCosine;
	}
	
	// ������ ������
	if (CMathUtility::Refract(InRay.GetDirection(), vOutwardNormal, fNiOverNt, vScattered))
	{
		fReflectProb = CMathUtility::Schlick(fCosine, m_fRefIdx);
	}
	// �ݻ� �Ǿ�����
	else
	{
		fReflectProb = 1.0f;
	}

	// Ȯ�� �Լ��� ���� �ݻ縦 �������� ������ �ݻ�� ������
	// ���ÿ� �Ͼ�� �����̴� (���� 100�ۼ�Ʈ �ݻ���� ����)
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
