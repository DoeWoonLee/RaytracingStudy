#pragma once

#include "Pdf.h"
#include "OrthoNormalBase.h"
#include "MathUtility.h"

class CosinePdf : public IPdf
{
public:
	CosinePdf(const vec3& w) { m_UVW.BuildFromW(w); }
	virtual float Value(const vec3& vDirection) const override;
	virtual vec3 Generate() const override;

private:
	OrthoNormalBase m_UVW;
};