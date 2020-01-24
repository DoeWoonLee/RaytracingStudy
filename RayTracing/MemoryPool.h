#pragma once

#include "Macros.h"

#include <vector>
#include <list>
#include <mutex>

#define MEMORYPOOLUSE

class CMemoryPool
{
// ================== Single Tone ==================
public:
	DECLARE_SINGLETON(CMemoryPool)
private:
	CMemoryPool();
	~CMemoryPool();
	// Inner Sturcts
private:
	struct MEMORY_PAGE {
		MEMORY_PAGE(UINT uiMemorySize) : pMemory((BYTE*)malloc(uiMemorySize)), uiSize(uiMemorySize), uiEnd(0)	{	}
		~MEMORY_PAGE() { free(pMemory); }
		BYTE* pMemory;
		UINT uiSize;
		UINT uiEnd;
	};
public:
	enum MEMORY_INDEX {
		OBJECT,
		BVH_TREE,
		KD_TREE,
		MEMORY_INDEX_END
	};

	template<class T, class ... Args>
	static T* New(MEMORY_INDEX eType, Args ...Arguments)
	{
		void* pMemory = CMemoryPool::GetInstance()->GetMemory(eType, sizeof(T), alignof(T));
		T* pNewData = new (pMemory) T(Arguments...);
		_ASSERT(pNewData != nullptr);
		return pNewData;
	}
public: // Public Functions 
	void ReadyMemoryPool(void);
	BYTE* GetMemory(MEMORY_INDEX eMemoryType, UINT uiNeedSize, uint8_t alignment);
private: // Private Function

private: // Member Values
	std::vector<std::list<MEMORY_PAGE*>> m_vecMemorys;
	std::mutex m_Mutex;

};

//#define DECLARE_CREATE_BY_MEMORYPOOL(CLASSTYPE, MEMORY_TYPE) \
//template<class... Args>									\
//static CLASSTYPE* Create(Args... Arguments)				\
//{														\
//	return CMemoryPool::New<CLASSTYPE>(MEMORY_TYPE, Arguments...);\
//}

#define DECLARE_CREATE_BY_MEMORYPOOL(CLASSTYPE, MEMORY_TYPE) \
template<class... Args>									\
static CLASSTYPE* Create(Args... Arguments)				\
{														\
	return new CLASSTYPE(Arguments...);\
}