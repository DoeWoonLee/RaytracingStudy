#pragma once

#include <DirectXMath.h>

using namespace DirectX;

typedef struct tagMatrix : public XMFLOAT4X4
{
	tagMatrix(void) {}
	explicit tagMatrix(XMMATRIX matSimd);
	tagMatrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
		: XMFLOAT4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
	explicit tagMatrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}

	tagMatrix Inverse(void);
	tagMatrix Identity(void);
	XMMATRIX ToSIMD(void) const;
	void LoadSIMD(XMMATRIX matSimd);
	float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
	float&      operator() (size_t Row, size_t Column) { return m[Row][Column]; }

	tagMatrix& operator= (const tagMatrix& Float4x4);

	tagMatrix operator+(const tagMatrix& Float4x4);
	tagMatrix operator-(const tagMatrix& Float4x4);
	tagMatrix operator*(const tagMatrix& Float4x4);

	void operator*=(const tagMatrix& Float4x4);
	void operator+=(const tagMatrix& Float4x4);
	void operator-=(const tagMatrix& Float4x4);
}_matrix;