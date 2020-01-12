#pragma once

#include "Vector3.h"

struct VERTEX {
	vec3 vPos;
	vec3 vNormal;
};
struct INDEX16 {
	unsigned short _1;
	unsigned short _2;
	unsigned short _3;
};
struct INDEX32 {
	unsigned int _1;
	unsigned int _2;
	unsigned int _3;
};
struct SUBSET {
	UINT uiFaceStart;
	UINT uiFaceCount;
};