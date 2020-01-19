#pragma once

#include "Vector3.h"
#include "Vector2.h"

struct VERTEX {
	vec3 vPos;
	vec3 vNormal;
	vec2 vUV;
};
struct INDEX16 {
	unsigned short _1;
	unsigned short _2;
	unsigned short _3;

	template<typename T>
	inline unsigned short& operator [](const T& iIndex)
	{
		return reinterpret_cast<unsigned short*>(this)[iIndex];
	}
};

struct INDEX32 {
	unsigned int _1;
	unsigned int _2;
	unsigned int _3;

	template<typename T>
	inline unsigned int& operator [](const T& iIndex)
	{
		return reinterpret_cast<unsigned int*>(this)[iIndex];
	}
};
struct SUBSET {
	UINT uiFaceStart;
	UINT uiFaceCount;
};