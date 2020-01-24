#include "stdafx.h"
#include "Utility.h"
#include <float.h>
#include "BVHTree.h"

int g_MaxDepth = 50;

void CUtility::GetSkyColor(float& y, vec3& vOut)
{
	/*XMVECTOR vLerp1 = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
	XMVECTOR vLerp2 = DirectX::XMVectorSet(0.5f, 0.7f, 1.f, 0.f);
	float t = 0.5f * (y + 1.0f);

	XMStoreFloat3(&vOut, (1.0f - t) * vLerp1 + t * vLerp2);*/

	vOut = vec3(0.f, 0.f, 0.f);
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
	float fPdf = 0.f;

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
			vecFieldObject[iHitIdx]->Scatter(Record, Ray, outRay, vNewColor, fPdf);
			vColor *= vNewColor;
			Ray = outRay;
		}
	}
	vColor /= (float)iDepth;
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
	// Probability Density Function
	float fPdf = 0.f;

	HitRecord Record;
	CFieldObject* pHitedObject = nullptr;

	for (; iDepth < g_MaxDepth && true == bHit; ++iDepth)
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
			break;
		}
		else
		{
			vEmittedLight = pHitedObject->Emitted(Record.vPos);

			if (pHitedObject->Scatter(Record, Ray, outRay, vNewColor, fPdf))
			{
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
