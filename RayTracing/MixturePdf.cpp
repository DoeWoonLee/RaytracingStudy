#include "stdafx.h"
#include "MixturePdf.h"
#include "MathUtility.h"
#include "Resources.h"
#include "Transform.h"

MixturePdf::MixturePdf(const CResources* p0, IPdf * p1)
{
	m_p0 = p0;
	m_p1 = p1;
}

float MixturePdf::Value(const vec3& vHitPos,
	const vec3& vScale,
	const vec3 & vLocalDir,
	const vec3& vDirection) const
{
	return 0.5f * m_p0->PdfValue(vHitPos, vScale, vLocalDir, vDirection) + 0.5f * m_p1->Value(vDirection);
}

vec3 MixturePdf::Generate(const vec3 & vWorldPos, const CTransform* pTransform, bool& bType) const
{

	/*if (CMathUtility::frand0to1() < 0.5f)
	{
		bType = true;
		return m_p0->GenerateRandDirinRes(vWorldPos, pTransform);
	}
	else
	{*/
		bType = false;
		return m_p1->Generate();
	//}
		
}
