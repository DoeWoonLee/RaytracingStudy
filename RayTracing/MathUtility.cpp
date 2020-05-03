#include "stdafx.h"
#include "MathUtility.h"

thread_local UINT g_uiState = 2;

UINT & CMathUtility::XorShift32(void)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.

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
	// ���� 1 ���� / ���� 2 ���� 
	// �� ������ ������ ��Ģ�� n1/n2 = sin2/sin1�� �����Ѵ�. (n�� ���� ���)
	// ���� ���� ���� r�� ���ϴ� ���� r =  (v + N * cos1) * sin2 / sin1  - N * cos2 (v �� �Ի� ���� )
	// r = (n1 / n2) * (v + N * cos1) - N * cos2 �̴�.
	// ���⼭ �������� r�� cos2�� �Ǵµ� cos2�� cos1�� ���� ������ ������ �����ϴ�.
	// �̶� sin^2 + cos^2 = 1�� �������� ���ǰ� �Ǹ鼭 ��Ʈ�� �������� �̴� �Ǻ����� �ۿ�����
	// �����ݻ�� ������ ���κ��� ������ �ȴ�.

	float fDt = vec3::Dot(vRayDir, vNormal);


	float fDiscriminant = 1.0f - fNiOverNt * fNiOverNt * (1.f - fDt * fDt);

	sNormal = vNormal.ToSIMD();
	sRayDir = vRayDir.ToSIMD();

	// ������ �Ͼ��
	if (fDiscriminant > 0.f)
	{
		vRefracted = XMVector3Normalize( fNiOverNt * (sRayDir - sNormal * fDt) - sNormal * sqrtf(fDiscriminant));
		
		return true;
	}
	else // �ݻ簡 �Ͼ��.
		return false;
}

float CMathUtility::Schlick(const float & fCosine, const float & fRefIdx)
{
	// ������ ȿ���� ���Ҷ� ����ϴ� �ٻ� �Լ�
	// fCosine�� �븻�� �Ի籤���� ������
	// Ư�� ����(�������)�� ���ؼ� ���� ������
	// ���ؼ� ���ݻ簡 �Ͼ�� ������ �����Ҷ� ���

	float r0 = (1.f - fRefIdx) / (1.f + fRefIdx);
	r0 = r0 * r0;
	return r0 + (1.f - r0) * powf(1.f - fCosine, 5);
}

vec3 CMathUtility::NormalByTri(const vec3 & v0, const vec3 & v1, const vec3 & v2)
{
	return vec3(XMPlaneFromPoints(v0.ToSIMD(), v1.ToSIMD(), v2.ToSIMD()));
}
