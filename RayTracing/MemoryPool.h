#pragma once

#include <vector>
#include <list>
#include <mutex>



class CMemoryPool
{
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
		MEMORY_INDEX_END
	};
	template<typename T>
	class Allocate
	{
	public:
		template<class... Args>
		inline static T* NEW (MEMORY_INDEX eType, Args... Arguments)
		{
			BYTE* pMemory = CMemoryPool::GetInstance()->GetMemory(eType, sizeof(T));
			return new (pMemory) T(Arguments...);
		}
	};
public: // Public Functions 
	void DefaultSetting(void);
	BYTE* GetMemory(MEMORY_INDEX eMemoryType, UINT uiNeedSize);
private: // Private Function

private: // Member Values
	std::vector<std::list<MEMORY_PAGE*>> m_vecMemorys;
	std::mutex m_Mutex;
// ================== Single Tone ==================
private:
	static CMemoryPool* m_pInstance;
private:
	explicit CMemoryPool();
	~CMemoryPool();
public:
	static CMemoryPool* GetInstance();
	void DestroyInstance(void);

};

#define DECLARE_CREATE_BY_MEMORYPOOL(CLASSTYPE, MEMORY_TYPE) \
template<class... Args>									\
static CLASSTYPE* Create(Args... Arguments)				\
{														\
	return CMemoryPool::Allocate<CLASSTYPE>::NEW(MEMORY_TYPE, Arguments...);\
}