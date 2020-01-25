#pragma once

#include "Pdf.h"

class CTransform;
class CResources;
class MixturePdf
{
public:
	MixturePdf(const CResources* p0, IPdf* p1);
	float Value(const vec3& vHitPos,
		const vec3& vScale,
		const vec3 & vLocalDir,
		const vec3& vDirection) const;
	vec3 Generate(const vec3 & vWorldPos, const CTransform* pTransform, bool& bType) const;
private:
	const CResources* m_p0;
	IPdf* m_p1;
};