#include "stdafx.h"
#include "LogMgr.h"

IMPLEMENT_SINGLETON(CLogMgr)

CLogMgr::CLogMgr(void) : m_pOstream(nullptr)
{
	
}

CLogMgr::~CLogMgr(void)
{
	Release();
	
}

void CLogMgr::ReadyLogMgr(void)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	char buffer[256];


	snprintf(buffer, 256, ".\\Logs\\Log%d-%d-%d %d-%d-%d.txt", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	std::locale::global(std::locale("Korean"));

	m_pOstream = new std::wofstream(buffer, std::ofstream::out);

}

void CLogMgr::WriteLog(const TCHAR * pLog)
{
	std::lock_guard<std::mutex> lock(m_LogLock);

	(*m_pOstream) << pLog << std::endl;
}

void CLogMgr::Release(void)
{
	_ASSERT( m_pOstream != nullptr);

	m_pOstream->close();
	SAFE_DELETE(m_pOstream);
}
