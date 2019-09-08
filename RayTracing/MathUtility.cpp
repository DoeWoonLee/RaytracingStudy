#include "stdafx.h"
#include "MathUtility.h"

thread_local UINT g_uiState = 1;

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

vec3 CMathUtility::RandUnitSphereVector(void)
{
	vec3 vInnerUnitSphere;
	do {
		vInnerUnitSphere.x = frand0to1() * 2.f - 1.f;
		vInnerUnitSphere.y = frand0to1() * 2.f - 1.f;
		vInnerUnitSphere.z = frand0to1() * 2.f - 1.f;

	} while (vInnerUnitSphere.LengthSquared() >= 1.f);

	return vInnerUnitSphere;
}
