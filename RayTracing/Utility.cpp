#include "stdafx.h"
#include "Utility.h"

void CUtility::GetSkyColor(float& y, vec3& vOut)
{
	XMVECTOR vLerp1 = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
	XMVECTOR vLerp2 = DirectX::XMVectorSet(0.5f, 0.7f, 1.f, 0.f);
	float t = 0.5f * (y + 1.0f);

	XMStoreFloat3(&vOut, (1.0f - t) * vLerp1 + t * vLerp2);
}

void CUtility::GetPixelColor(CRay & Ray)
{
	XMVECTOR sColor = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);

	// if Hit

}

bool  CUtility::CalculateColor(CRay & Ray)
{
	float fRadius = 0.5f;
	XMVECTOR sCenter = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
	//Spehre Check
	XMVECTOR sDir = Ray.GetSDirection();
	XMVECTOR sOrigin = Ray.GetSOrigin();

	XMVECTOR R = sOrigin - sCenter;

	float A = DirectX::XMVector3Dot(sDir, sDir).m128_f32[0] ;
	float B = 2.f * DirectX::XMVector3Dot(sDir,	 R).m128_f32[0];
	float C = DirectX::XMVector3Dot(R, R).m128_f32[0] - fRadius * fRadius;

	float fDiscriminant = B * B - 4.f * A * C;

	return fDiscriminant > 0.f;
}
