#pragma once

#include "Macros.h"
#include <vector>
#include "Ray.h"
#include "HitRecord.h"

#define EMITTEDOBJECTMGR EmittedObjectsMgr::GetInstance()


class CFieldObject;
class EmittedObjectsMgr
{
	DECLARE_SINGLETON(EmittedObjectsMgr)
public:
	EmittedObjectsMgr(void);
	~EmittedObjectsMgr(void);

	void AddEmittedObject(CFieldObject* pFieldObject);
	void GenerateScatterRay(CRay& vOutRay,
		float& fPdfValue,
		const vec3& vHitPos,
		const ScatterRecord& sRec) const;
private:
	std::vector<CFieldObject*> m_EmittedObjects;

};