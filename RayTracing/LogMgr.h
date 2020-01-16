#pragma once

#include "Macros.h"
#include <fstream>
#include <mutex>

class CLogMgr
{
	DECLARE_SINGLETON(CLogMgr)
	CLogMgr(void);
	~CLogMgr(void);
public:
	void ReadyLogMgr(void);
	void WriteLog(const TCHAR* pLog);

	template<class ... Args>
	void WriteLog(const TCHAR* pFormat, Args... arguments);
private:
	void Release(void);
private:
	std::wofstream* m_pOstream;
	std::mutex m_LogLock;
};

#define LOGMGR CLogMgr::GetInstance()


template<class ...Args>
inline void CLogMgr::WriteLog(const TCHAR * pFormat, Args ...arguments)
{
	WCHAR g_WBuffer[256];
	int g_WBufferSize = 255;

	_snwprintf_s(g_WBuffer, g_WBufferSize, pFormat, arguments...);

	std::lock_guard<std::mutex> lock(m_LogLock);
	(*m_pOstream) << g_WBuffer << std::endl;
}
