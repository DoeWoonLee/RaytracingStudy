#pragma once

#include <thread>
#include <queue>
#include <vector>
#include <mutex>


//#define FUNCTION void (*)(void*)
typedef void (*FUNCTION)(void*);
#define CALL_FUNC_PAR1(Function, Par1) Function(Par1)

struct TASK
{
public:
	TASK(void) : Functor(nullptr), pData(nullptr) {}
	TASK(FUNCTION _Functor, void* _pData) : Functor(_Functor), pData(_pData) {}
	FUNCTION Functor;
	void* pData;
};
class CThreadPool
{
public:
	static void InitThreads(void);

	static void Run(int iIndex);
	static bool RunMain(void);

	static void Push(TASK tTask);
	static void WakeThreads(void);
	static void MainWait(void);

	static bool CheckNowRunning(void);
public:
	static std::mutex g_PublicMutex;
private:
	static std::queue<TASK> g_TaskQueue;
	static std::thread* g_Threads;
	static std::mutex g_mutex;
	static std::mutex* g_pThreadMutex;

	static int g_iRunningThread;
	static int g_iTaskCnt;

	static std::condition_variable g_WorkerCV;
	static std::condition_variable g_MainCV;
};
