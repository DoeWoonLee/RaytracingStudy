#include "stdafx.h"
#include "MemoryPool.h"



#define MEMORY_PAGE_SIZE 16384


IMPLEMENT_SINGLETON(CMemoryPool)


CMemoryPool::CMemoryPool()
{
}

CMemoryPool::~CMemoryPool()
{

	for (int i = 0; i < MEMORY_INDEX_END; ++i)
	{
		for each (MEMORY_PAGE* var in m_vecMemorys[i])
		{
			delete var;
			var = nullptr;
		}
	}

}

void CMemoryPool::ReadyMemoryPool(void)
{
	m_vecMemorys.resize(MEMORY_INDEX_END);

	for (int i = 0; i < MEMORY_INDEX_END; ++i)
	{
		m_vecMemorys[i].push_back(new MEMORY_PAGE(MEMORY_PAGE_SIZE));
	}
}
// 임시로 이렇게 둠 =====
uint8_t AlignForwardAdjustment(const void* pAddress, uint8_t alignment)
{
	uint8_t adjustment = alignment - (reinterpret_cast<size_t>(pAddress) & static_cast<size_t>(alignment - 1));

	if (adjustment == alignment)
		return 0;

	return adjustment;
}
// ===============
BYTE * CMemoryPool::GetMemory(MEMORY_INDEX eMemoryType, UINT uiNeedSize, uint8_t alignment)
{
	auto Tail = m_vecMemorys[eMemoryType].rbegin();
	MEMORY_PAGE* pMemory = (*Tail);

	BYTE* pStartMemory = nullptr;
	{
		std::lock_guard<std::mutex> GetMemoryLock(m_Mutex);

		uint8_t adjustment =  AlignForwardAdjustment(&pMemory->pMemory[pMemory->uiEnd], alignment);
		// 공간 부족하면 새로 할당
		if (pMemory->uiSize - pMemory->uiEnd < uiNeedSize + adjustment)
		{
			pMemory = new MEMORY_PAGE(MEMORY_PAGE_SIZE);
			m_vecMemorys[eMemoryType].push_back(pMemory);

			adjustment = AlignForwardAdjustment(pMemory->pMemory, alignment);
		}

		// alignment
		
		uintptr_t alignAddress = reinterpret_cast<uintptr_t>(pMemory->pMemory) + pMemory->uiEnd + adjustment;
		pStartMemory = reinterpret_cast<BYTE*>(alignAddress);
		//pStartMemory = &pMemory->pMemory[pMemory->uiEnd + adjustment];

		pMemory->uiEnd += uiNeedSize + adjustment;
	}
	

	return pStartMemory;
}
