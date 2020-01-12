#include "stdafx.h"
#include "ConstantMedium.h"
#include "MathUtility.h"

CConstantMedium::CConstantMedium(const CResources * pResource, const float & fDensity): m_pResource(pResource), m_fDensity(fDensity)
{
	MakeMinMax();
}

void CConstantMedium::MakeMinMax()
{
	m_vMin = m_pResource->Min();
	m_vMax = m_pResource->Max();
}

bool CConstantMedium::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord) const
{
	bool db = db = false;
	HitRecord rec1, rec2;
	float fMinTp = -FLT_MIN;
	float fMaxTp = FLT_MAX;
	if (m_pResource->Hit(inRay, fMinTp, fMaxTp, rec1)) {
		fMinTp = rec1.fTime + 0.0001f;
		if (m_pResource->Hit(inRay, fMinTp, fMaxTp, rec2))
		{
			if (rec1.fTime < fMin)
				rec1.fTime = fMin;
			if (rec2.fTime > fMax)
				rec2.fTime = fMax;

			if (rec1.fTime > rec2.fTime) return false;
			if (rec1.fTime < 0.f) rec1.fTime = 0.f;

			float fDistanceInsideBoundary = rec2.fTime - rec1.fTime;
			float fHitDist = -(1.f / m_fDensity) * log(CMathUtility::frand0to1());
			if (fHitDist < fDistanceInsideBoundary)
			{
				hitRecord.fTime = rec1.fTime + fHitDist;
				hitRecord.vNormal = vec3(1.f, 0.f, 0.f);

				return true;
			}
		}
	}

	return false;
}
