#pragma once

#include "Material.h"
#include "MemoryPool.h"

class ITexture;
class CLambertain : public CMaterial
{
public:
	explicit CLambertain(void);
	explicit CLambertain(vec3& vAlbedo);
	explicit CLambertain(ITexture* pTexture);
	virtual ~CLambertain(void);
	virtual bool Scatter(HitRecord& Record, const CRay& InRay, CRay& OutRay, vec3& vColor, float& fPdf)const override;
	virtual float ScatteringPdf(const CRay& inRay, const CRay& OutRay, const HitRecord& hitRecord)override;

	DECLARE_CREATE_BY_MEMORYPOOL(CLambertain, CMemoryPool::OBJECT)
private:
	ITexture* m_pAlbedo;
};