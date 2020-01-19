#include "stdafx.h"
#include "ConstantTexture.h"

const vec3& ConstantTexture::Value(const float & u, const float & v, const vec3 & p) const
{
	return m_vColor;
}
