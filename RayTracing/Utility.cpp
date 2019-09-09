#include "stdafx.h"
#include "Utility.h"
#include <float.h>

void CUtility::GetSkyColor(float& y, vec3& vOut)
{
	XMVECTOR vLerp1 = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
	XMVECTOR vLerp2 = DirectX::XMVectorSet(0.5f, 0.7f, 1.f, 0.f);
	float t = 0.5f * (y + 1.0f);

	XMStoreFloat3(&vOut, (1.0f - t) * vLerp1 + t * vLerp2);
}


void CUtility::GetPixelColor(CRay & Ray, std::vector<CFiledObject*>& vecFieldObject, vec3& vOutColor)
{
	CRay outRay;
	float fMin = 0.000001f;
	float fMax = FLT_MAX;
	vec3 vColor = vec3(1.f, 1.f, 1.f);
	vec3 vEnvironmentColor;

	bool bHit = true;
	int iDepth = 0;
	for (; iDepth < 50 && true == bHit; ++iDepth)
	{
		fMax = FLT_MAX;
		bHit = false;
		for (size_t i = 0; i < vecFieldObject.size(); ++i)
		{
			 //Ãæµ¹ µÆÀ»¶§
			if (vecFieldObject[i]->Hit(Ray, outRay, fMin, fMax, vColor))
			{
				bHit = true;

			}
		}
		if (false == bHit)
		{
			
			float fRayY = Ray.GetDirection().y;

			GetSkyColor(fRayY, vEnvironmentColor);
			vColor *= vEnvironmentColor;

			break;
			
		}
		Ray = outRay;
	}
	//vColor /= (float)iDepth;
	vOutColor += vColor;

}
