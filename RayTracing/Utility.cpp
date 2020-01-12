#include "stdafx.h"
#include "Utility.h"
#include <float.h>
#include "BVHTree.h"

void CUtility::GetSkyColor(float& y, vec3& vOut)
{
	XMVECTOR vLerp1 = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
	XMVECTOR vLerp2 = DirectX::XMVectorSet(0.5f, 0.7f, 1.f, 0.f);
	float t = 0.5f * (y + 1.0f);

	XMStoreFloat3(&vOut, (1.0f - t) * vLerp1 + t * vLerp2);
}


void CUtility::GetPixelColor(CRay & Ray, std::vector<CFieldObject*>& vecFieldObject, vec3& vOutColor)
{
	CRay outRay;
	float fMin = 0.0001f;
	float fMax = FLT_MAX;
	vec3 vColor = vec3(1.f, 1.f, 1.f);
	vec3 vNewColor;

	bool bHit = true;
	int iDepth = 0;
	size_t iHitIdx = -1;

	HitRecord Record;

	for (; iDepth < 10 && true == bHit; ++iDepth)
	{
		fMax = FLT_MAX;
		bHit = false;

		for (size_t i = 0; i < vecFieldObject.size(); ++i)
		{
			 //Ãæµ¹ µÆÀ»¶§
			if (vecFieldObject[i]->Hit(Record, Ray, fMin, fMax))
			{
				bHit = true;
				iHitIdx = i;
			}
		}
		if (false == bHit)
		{
			
			float fRayY = Ray.GetDirection().y;

			GetSkyColor(fRayY, vNewColor);
			vColor *= vNewColor;

			break;
		}
		else
		{
			vecFieldObject[iHitIdx]->Scatter(Record, Ray, outRay, vNewColor);
			vColor *= vNewColor;
			Ray = outRay;
		}
	}
	//vColor /= (float)iDepth;
	vOutColor += vColor;

}

void CUtility::GetPixelColor(CRay & Ray, CBVHTree * pBVHTree, vec3 & vOutColor)
{
	CRay outRay;
	float fMin = 0.0001f;
	float fMax = FLT_MAX;
	vec3 vColor = vec3(1.f, 1.f, 1.f);
	vec3 vNewColor;
	vec3 vEmittedLight;

	bool bHit = true;
	int iDepth = 0;
	size_t iHitIdx = -1;

	HitRecord Record;
	CFieldObject* pHitedObject = nullptr;

	for (; iDepth < 50 && true == bHit; ++iDepth)
	{
		fMax = FLT_MAX;
		bHit = false;

		if (pBVHTree->Hit(Ray, fMin, fMax, Record, &pHitedObject))
		{
			bHit = true;
		}

		if (false == bHit)
		{

			float fRayY = Ray.GetDirection().y;

			GetSkyColor(fRayY, vNewColor);
			vColor *= vNewColor;
			//vColor *= vec3(0.0f, 0.0f, 0.0f);
			break;
		}
		else
		{
			vEmittedLight = pHitedObject->Emitted(Record.vPos);

			if (pHitedObject->Scatter(Record, Ray, outRay, vNewColor))
			{
				//float fCos = fmax(-vec3::Dot(g_vCamDir, outRay.GetDirection()), 0.0f);
				//fCos = powf(fCos, 22.f);
				//vNewColor = vNewColor * (1.f - fCos) + vec3(1.5f, 1.5f, 1.5f) * (fCos);

				vColor *= vNewColor + vEmittedLight;
				Ray = outRay;
			}
			else
			{ 
				vColor *= vEmittedLight;
				break;
			}
			
		}
	}
	//vColor /= (float)iDepth;
	vOutColor += vColor;
}
