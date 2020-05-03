#include "stdafx.h"
#include "RectangleXY.h"
#include "MathUtility.h"
#include "Transform.h"

CRectangleXY::CRectangleXY(const float & fWidth, const float & fHeight):
	m_fWidth(fWidth), m_fHeight(fHeight)
{
	MakeMinMax();
}

CRectangleXY::~CRectangleXY()
{
}

void CRectangleXY::MakeMinMax()
{
	m_vMin.x = -m_fWidth / 2.f;
	m_vMin.y = m_fHeight / 2.f;

	m_vMax.x = -m_vMin.x;
	m_vMax.y = -m_vMin.y;
}

bool CRectangleXY::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord) const
{
	float t = -inRay.GetOrigin().z / inRay.GetDirection().z;
	if (t > fMax || t < fMin) return false;

	vec3 vPos = inRay.PointAtParameter(t);

	if (-m_fHeight * 0.5f > vPos.y || vPos.y > m_fHeight *0.5f)
		return false;
	if (-m_fWidth * 0.5f > vPos.x || vPos.x > m_fWidth * 0.5f)
		return false;

	hitRecord.fTime = t;
	hitRecord.vNormal = vec3(0.f, 0.f, 1.f);
	hitRecord.vBiNormal = vec3(1.0f, 0.f, 0.f);
	hitRecord.vTangent = vec3(0.0f, 1.f, 0.f);

	hitRecord.vPos = vPos;

	hitRecord.vUV.x = vPos.x + 0.5f;
	hitRecord.vUV.y = vPos.y + 0.5f;

	return true;
}

// HitPos, OutRayDir => Local Value
float CRectangleXY::PdfValue(const vec3 & vHitPos,//Local
	const vec3& vScale,
	const vec3 & vOutRayDir, // Local
	const vec3& vOutWRayDir) const
{
	HitRecord hitRecrod;
	float fMin = 0.001f;
	float fMax = FLT_MAX;
	// Local
	if (this->Hit(CRay(vHitPos, vOutRayDir, true), fMin, fMax, hitRecrod))
	{
		// 죄다 로컬에서 처리
		float fArea = m_fHeight * m_fWidth;
		float fDistanceSquared = hitRecrod.fTime * hitRecrod.fTime * vOutRayDir.LengthSquared();
		float fCosine = fabs(vec3::Dot(vOutRayDir, hitRecrod.vNormal) / vOutRayDir.Length());

		return fDistanceSquared / (fCosine * fArea);
	}

	return 0.0f;
}

// vHitPos = World
// 무조건 월드에서 이루어 져야함 이라는 거지
vec3 CRectangleXY::GenerateRandDirinRes(const vec3 & vWorldPos,
	const CTransform* pTransform) const
{
	float fHalfWidth = m_fWidth * 0.5f;
	float fHalfHeight = m_fHeight * 0.5f;

	vec3 vRandomPoint = vec3(
		-fHalfWidth + CMathUtility::frand0to1() * m_fWidth,
		-fHalfHeight + CMathUtility::frand0to1() * m_fHeight,
		0.f);

	pTransform->WorldPos(vRandomPoint);

	return vRandomPoint - vWorldPos;
}
