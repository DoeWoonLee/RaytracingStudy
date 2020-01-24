#pragma once

#include "Resources.h"
#include <string>
#include <vector>

#include "MemoryPool.h"
#include "Vertex.h"

class CKDTree;
class CMesh : public CResources
{
public:
	CMesh(std::string strFilePath);
protected:
	virtual ~CMesh(void);
public:
	virtual void MakeMinMax();
	void SetMeshInfo(const std::vector<VERTEX>& vecVertices,
		const void* pIndices,
		const bool& b16bit, 
		const std::vector<SUBSET>& vecSubset);
public:
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord)const;


	DECLARE_CREATE_BY_MEMORYPOOL(CMesh, CMemoryPool::OBJECT)

private:
	std::vector<VERTEX> m_vecVertices;
	void* m_pIndices;
	UINT m_uiIdxCnt;
	std::vector<SUBSET> m_vecSubset;
	bool m_b16bit;
	CKDTree* m_pKDTree = nullptr;
};

