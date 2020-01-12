#include "stdafx.h"
#include "Triangle.h"
#include "MathUtility.h"

CTriangle::CTriangle() 
{
	m_vPos[TRI_1] = vec3(-1.f, 0.f, 0.f);
	m_vPos[TRI_2] = vec3(0.f, 1.f, 0.f);
	m_vPos[TRI_3] = vec3(1.f, 0.f, 0.f);

	m_vNormal = CMathUtility::NormalByTri(m_vPos[TRI_1], m_vPos[TRI_2], m_vPos[TRI_3]);


}

CTriangle::CTriangle(vec3 v0, vec3 v1, vec3 v2)
{
	m_vPos[TRI_1] = v0;
	m_vPos[TRI_2] = v1;
	m_vPos[TRI_3] = v2;

	m_vNormal = CMathUtility::NormalByTri(m_vPos[TRI_1], m_vPos[TRI_2], m_vPos[TRI_3]);
}

void CTriangle::MakeMinMax()
{
	float fMin = 0.f;


	for (int i = 0; i < TRI_END; ++i)
	{
		for (int j = 0; j < AXIS_END; ++j)
		{
			if (m_vPos[i][j] < m_vMin[j])
				m_vMin[j] = m_vPos[i][j];

			if (m_vPos[i][j] > m_vMax[j])
				m_vMax[j] = m_vPos[i][j];
		}
		
	}
}

bool CTriangle::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord)const
{

	float fDiscriminant = vec3::Dot(m_vNormal, inRay.GetDirection());

	// 광선과 삼각형이 평행하는 경우
	if (fabs(fDiscriminant) < 0.00001f)
		return false;

	// fD가 내적으로 나오는 이유
	// AX + BY + CZ + D = 0이 평면 방정식이다.
	// 이때 A, B, C는 평면의 법선 벡터이고 이는 Normal을 뜻한다.
	// 이러한 경우 A, B, C의 값은 알고 있는 경우이니, 삼격형의 세 점중
	// 하나를 선택해 대입하면 fD를 알 수 있다.
	// 이 대입했을때의 연산이 내적과 동일하다.
	float fD = -vec3::Dot(m_vPos[TRI_1], m_vNormal);

	// P = O  + t*R
	// Ax + By + Cz + D = 0;
	// A(Ox + tRx) + B(Oy +tRy) + C(Oz + tRz) + D = 0
	// AOx + AtRx + BOy + BtRy + Coz + CtRz + D = 0
	// t(ARx + BRy + CRz) + AOx + BOy + COz + D = 0
	// t = - (AOx + BOy + COz + D) / (ARx + BRy + CRz )
	// t = - ( dot(N,O) + D ) / dot(N,R);
	float t = -(vec3::Dot(m_vNormal, inRay.GetOrigin()) + fD) / (fDiscriminant);

	if (fMin >= t || t >= fMax)
	{
		return false;
	}
	vec3 vPos = inRay.PointAtParameter(t);

	// 삼각형 내부에 있는지 검사
	// 외적을 이용해서 법선 벡터를 구한다.
	// 이때 이 법선 벡터가 노말과 전부 같은 방향이어야지
	// 삼각형 내부를 통과한다고 판정한다.
	vec3 vDir = vPos - m_vPos[TRI_1];
	vec3 vEdge = m_vPos[TRI_2] - m_vPos[TRI_1];
	if (vec3::Dot(m_vNormal, vec3::Cross(vEdge, vDir)) <= 0.f)
		return false;

	vDir = vPos - m_vPos[TRI_2];
	vEdge = m_vPos[TRI_3] - m_vPos[TRI_2];
	if (vec3::Dot(m_vNormal, vec3::Cross(vEdge, vDir)) <= 0.f)
		return false;

	vDir = vPos - m_vPos[TRI_3];
	vEdge = m_vPos[TRI_1] - m_vPos[TRI_3];
	if (vec3::Dot(m_vNormal, vec3::Cross(vEdge, vDir)) <= 0.f)
		return false;

	hitRecord.vNormal = m_vNormal;
	hitRecord.fTime = t;

	return true;
}
