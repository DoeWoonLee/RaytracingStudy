#pragma once

#include "Texture.h"

class ConstantTexture : public Texture
{
public:
	ConstantTexture() {}
	ConstantTexture(const vec3& c) : m_vColor(c) {}
	virtual ~ConstantTexture() {}
	const vec3& Value(const float& u, const float& v, const vec3& p)const override;
private:
	vec3 m_vColor;
};