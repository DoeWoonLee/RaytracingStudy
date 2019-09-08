#pragma once
#pragma  warning (disable : 4275)

#include <DirectXMath.h>
using namespace DirectX;


typedef struct tagVector3 : public XMFLOAT3
{
	tagVector3(float fX = 0.f, float fY = 0.f, float fZ = 0.f)
		: XMFLOAT3(fX, fY, fZ) {};
	explicit tagVector3(XMVECTOR vSimd);
	explicit tagVector3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}
	void Normalize(void);
	float Length(tagVector3& vVector);
	float Length(void);
	float LengthSquared(void);

	const tagVector3& TransformNormal(XMMATRIX mat);
	const tagVector3& TransformCoord(XMMATRIX mat);
	//tagVector3& operator= (const tagVector3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }

	XMVECTOR ToSIMD(void) const;
	void LoadSIMD(XMVECTOR vSimd);

	tagVector3 operator+(const tagVector3& AddedVec3);
	tagVector3 operator-(const tagVector3& AddedVec3);

	void operator+=(const tagVector3& AddedVec3);
	void operator*=(const tagVector3& AddedVec3);
	void operator/=(const tagVector3& AddedVec3);
	void operator-=(const tagVector3& AddedVec3);

	void operator*=(const float& Float);
	void operator/=(const float& Float);

	void operator+=(const XMVECTOR& Vector);
	void operator*=(const XMVECTOR& Vector);
	void operator/=(const XMVECTOR& Vector);
	void operator-=(const XMVECTOR& Vector);

	bool operator==(const tagVector3& Vector);

	tagVector3 operator*(const float& fFloat);
	tagVector3 operator/(const float& fFloat);

	tagVector3 operator+(const float& fFloat);
	tagVector3 operator-(const float& fFloat);

	friend tagVector3 operator*(const float& fFloat, const tagVector3& Vector);
	friend tagVector3 operator/(const float& fFloat, const tagVector3& Vector);

}vec3;
inline tagVector3 operator*(const float& fFloat, const tagVector3& Vector)
{
	XMVECTOR FVector = XMLoadFloat3(&Vector);
	FVector = XMVectorScale(FVector, fFloat);

	return tagVector3(FVector);
}
inline tagVector3 operator/(const float& fFloat, const tagVector3& Vector)
{
	XMVECTOR FVector = XMVectorSet(fFloat, fFloat, fFloat, 0.f) / XMLoadFloat3(&Vector);

	return tagVector3(FVector);
}

