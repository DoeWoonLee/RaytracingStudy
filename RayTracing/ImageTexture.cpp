#include "stdafx.h"
#include "ImageTexture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"
ImageTexture::ImageTexture(const char * pFilePath, const vec2 & vScale) 
	: m_pNormal(nullptr), m_vScale(vScale)
{
	int comp;
	stbi_set_flip_vertically_on_load(true);
	m_pAlbedo = stbi_load(pFilePath, &m_iAWidth, &m_iAHeight, &comp, 0);
}

ImageTexture::ImageTexture(const char * pAlbedoFilePath, const char * pNormalFilePath, const vec2 & vScale)
	: m_vScale(vScale)
{
	int comp;
	stbi_set_flip_vertically_on_load(true);
	m_pAlbedo = stbi_load(pAlbedoFilePath, &m_iAWidth, &m_iAHeight, &comp, 0);
	stbi_set_flip_vertically_on_load(true);
	m_pNormal = stbi_load(pNormalFilePath, &m_iNWidth, &m_iNHeight, &comp, 0);
}

TexInfo ImageTexture::Value(const float & u, const float & v, const vec3 & p) const
{
	TexInfo texInfo;

	int i = int(u * (float)m_iAWidth * m_vScale.x);
	int j = int(v * (float)m_iAHeight * m_vScale.y - 0.001);

	i = i % m_iAWidth;
	j = j % m_iAHeight;

	if (i < 0)
		i = 0;

	if (j < 0)
		j = 0;

	if (i > m_iAWidth - 1)
		i = m_iAHeight - 1;

	if (j > m_iAWidth - 1)
		j = m_iAHeight - 1;

	float invD = 1.0f / 255.0f;

	texInfo.vAlbedo.x = int(m_pAlbedo[j *m_iAWidth * 3 + i * 3 + 0]) * invD;
	texInfo.vAlbedo.y = int(m_pAlbedo[j *m_iAWidth * 3 + i * 3 + 1]) * invD;
	texInfo.vAlbedo.z = int(m_pAlbedo[j *m_iAWidth * 3 + i * 3 + 2]) * invD;

	if (m_pNormal != nullptr)
	{
		i = int(u * (float)m_iNWidth * m_vScale.x);
		j = int(v * (float)m_iNHeight * m_vScale.y - 0.001f);

		i = i % m_iNWidth;
		j = j % m_iNHeight;

		if (i < 0)
			i = 0;

		if (j < 0)
			j = 0;

		if (i > m_iNWidth - 1)
			i = m_iNHeight - 1;

		if (j > m_iNWidth - 1)
			j = m_iNHeight - 1;

		invD = 1.0f / 255.0f;

		texInfo.vNormal.x = int(m_pNormal[j *m_iNWidth * 3 + i * 3 + 0]) * invD;
		texInfo.vNormal.y = int(m_pNormal[j *m_iNWidth * 3 + i * 3 + 1]) * invD;
		texInfo.vNormal.z = int(m_pNormal[j *m_iNWidth * 3 + i * 3 + 2]) * invD;

		texInfo.bBumpMap = true;
	}

	return texInfo;
}
