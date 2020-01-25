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
	virtual bool Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const override;
	virtual float ScatteringPdf(const HitRecord & hRec, const CRay& inRay, const CRay& OutRay)const override;

	DECLARE_CREATE_BY_MEMORYPOOL(CLambertain, CMemoryPool::OBJECT)
private:
	ITexture* m_pAlbedo;
};