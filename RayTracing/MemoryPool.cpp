#include "stdafx.h"
#include "MemoryPool.h"

#define MEMORY_PAGE_SIZE 2048

CMemoryPool* CMemoryPool::m_pInstance = nullptr;


void CMemoryPool::DefaultSetting(void)
{
	m_vecMemorys.resize(MEMORY_INDEX_END);

	for (int i = 0; i < MEMORY_INDEX_END; ++i)
	{
		m_vecMemorys[i].push_back(new MEMORY_PAGE(MEMORY_PAGE_SIZE));
	}
}

BYTE * CMemoryPool::GetMemory(MEMORY_INDEX eMemoryType, UINT uiNeedSize)
{
	auto Tail = m_vecMemorys[eMemoryType].rbegin();
	MEMORY_PAGE* pMemory = (*Tail);

	BYTE* pStartMemory = nullptr;

	{
		std::lock_guard<std::mutex> GetMemoryLock(m_Mutex);

		// 공간 부족하면 새로 할당
		if (pMemory->uiSize - pMemory->uiEnd < uiNeedSize)
		{
			pMemory = new MEMORY_PAGE(MEMORY_PAGE_SIZE);
			m_vecMemorys[eMemoryType].push_back(pMemory);
		}

		pStartMemory = &pMemory->pMemory[pMemory->uiEnd];

		pMemory->uiEnd += uiNeedSize;
	}
	

	return pStartMemory;
}

CMemoryPool::CMemoryPool()
{
}

CMemoryPool::~CMemoryPool()
{
}

void CMemoryPool::DestroyInstance(void)
{
	for (int i = 0; i < MEMORY_INDEX_END; ++i)
	{
		for each ( MEMORY_PAGE* var in m_vecMemorys[i])
		{
			delete var;
			var = nullptr;
		} 
	}

	m_pInstance = nullptr;
	delete this;
}

CMemoryPool * CMemoryPool::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new CMemoryPool();
	}
	return m_pInstance;
}
