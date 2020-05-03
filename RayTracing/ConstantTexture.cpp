#include "stdafx.h"
#include "ConstantTexture.h"

TexInfo ConstantTexture::Value(const float & u, const float & v, const vec3 & p) const
{
	return TexInfo(m_vColor);
}
