#include "stdafx.h"
#include "MathUtility.h"

thread_local UINT g_uiState = 2;

UINT & CMathUtility::XorShift32(void)
{
	// TODO: 여기에 반환 구문을 삽입합니다.

	g_uiState ^= g_uiState << 13;
	g_uiState ^= g_uiState >> 17;
	g_uiState ^= g_uiState << 15;

	return g_uiState;
}

float CMathUtility::frand0to1(void)
{
	return (XorShift32() & 0xFFFFFF) / 16777216.0f;
}

thread_local vec3 vInnerUnitSphere;
vec3 CMathUtility::RandUnitSphereVector(void)
{
	
	do {
		vInnerUnitSphere.x = frand0to1() * 2.f - 1.f;
		vInnerUnitSphere.y = frand0to1() * 2.f - 1.f;
		vInnerUnitSphere.z = frand0to1() * 2.f - 1.f;

	} while (vInnerUnitSphere.LengthSquared() >= 1.f);

	return vInnerUnitSphere;
}

vec3 CMathUtility::RandCosineDirection(void)
{
	float r1 = frand0to1();
	float r2 = frand0to1();
	float z = sqrtf(1.f - r2);
	float phi = 2.f * XM_PI*r1;
	float x = cosf(phi) * sqrtf(r2);
	float y = sinf(phi) * sqrtf(r2);
	return vec3(x, y, z);
}

vec3 CMathUtility::Reflect(const vec3 & vDir,const vec3 & vNormal, float& fDiscriminant)
{
	fDiscriminant = vec3::Dot(vDir, vNormal) * 2.f;


	return vec3(XMVector3Normalize(vDir.ToSIMD() - fDiscriminant * vNormal.ToSIMD()));
}
thread_local XMVECTOR sNormal;
thread_local XMVECTOR sRayDir;

bool CMathUtility::Refract(const vec3 & vRayDir, const vec3 & vNormal, const float & fNiOverNt, vec3 & vRefracted)
{
	// 영역 1 공기 / 영역 2 매질 
	// 위 가정시 스넬의 법칙은 n1/n2 = sin2/sin1을 만족한다. (n은 굴절 계수)
	// 굴절 방향 벡터 r을 구하는 식은 r =  (v + N * cos1) * sin2 / sin1  - N * cos2 (v 는 입사 광선 )
	// r = (n1 / n2) * (v + N * cos1) - N * cos2 이다.
	// 여기서 미지수는 r과 cos2가 되는데 cos2는 cos1에 대한 식으로 변경이 가능하다.
	// 이때 sin^2 + cos^2 = 1의 방정식이 사용되게 되면서 루트가 쓰여지고 이는 판별식의 작용으로
	// 완전반사와 굴절의 경계부분을 나누게 된다.

	float fDt = vec3::Dot(vRayDir, vNormal);


	float fDiscriminant = 1.0f - fNiOverNt * fNiOverNt * (1.f - fDt * fDt);

	sNormal = vNormal.ToSIMD();
	sRayDir = vRayDir.ToSIMD();

	// 굴절이 일어난다
	if (fDiscriminant > 0.f)
	{
		vRefracted = XMVector3Normalize( fNiOverNt * (sRayDir - sNormal * fDt) - sNormal * sqrtf(fDiscriminant));
		
		return true;
	}
	else // 반사가 일어난다.
		return false;
}

float CMathUtility::Schlick(const float & fCosine, const float & fRefIdx)
{
	// 프레넬 효과를 구할때 사용하는 근사 함수
	// fCosine은 노말과 입사광석의 내적값
	// 특정 매질(굴절상수)에 대해서 일정 각도에
	// 대해서 전반사가 일어나는 현상을 구현할때 사용

	float r0 = (1.f - fRefIdx) / (1.f + fRefIdx);
	r0 = r0 * r0;
	return r0 + (1.f - r0) * powf(1.f - fCosine, 5);
}

vec3 CMathUtility::NormalByTri(const vec3 & v0, const vec3 & v1, const vec3 & v2)
{
	return vec3(XMPlaneFromPoints(v0.ToSIMD(), v1.ToSIMD(), v2.ToSIMD()));
}
