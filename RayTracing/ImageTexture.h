#pragma once

#include "Texture.h"
#include "Vector2.h"

class ImageTexture : public ITexture
{
public:
	ImageTexture(const char* pFilePath, const vec2& vScale);
	ImageTexture(const char* pAlbedoFilePath,const char* pNormalFilePath, const vec2& vScale);
	virtual TexInfo Value(const float& u, const float& v, const vec3& p) const override;

private:
	BYTE* m_pAlbedo;
	int m_iAWidth;
	int m_iAHeight;
	int m_iNWidth;
	int m_iNHeight;


	BYTE* m_pNormal;
	vec2 m_vScale;
};