#include "stdafx.h"
#include "ThreadPool.h"

std::queue<TASK> CThreadPool::g_TaskQueue;
std::thread* CThreadPool::g_Threads;
std::mutex CThreadPool::g_mutex;
std::mutex* CThreadPool::g_pThreadMutex;
std::mutex CThreadPool::g_PublicMutex;

std::condition_variable CThreadPool::g_WorkerCV;
std::condition_variable CThreadPool::g_MainCV;
int CThreadPool::g_iRunningThread = 0;
int CThreadPool::g_iTaskCnt = 0;

void CThreadPool::InitThreads(void)
{


	int iThreadCnt = std::thread::hardware_concurrency() - 1;

	g_Threads = new std::thread[iThreadCnt];

	g_pThreadMutex = new std::mutex[iThreadCnt];

	for (int i = 0; i < iThreadCnt; ++i)
	{
		//g_pThreadMutex[i].lock();
		g_Threads[i].swap(std::thread(&CThreadPool::Run, i));
	}
}

void CThreadPool::Run(int iIndex)
{
	std::unique_lock<std::mutex> LockWorker(g_pThreadMutex[iIndex]);

	while (true)
	{
		// SelfWait
		g_mutex.lock();
		if (g_TaskQueue.empty())
		{
			g_mutex.unlock();
			g_WorkerCV.wait(LockWorker);
			g_mutex.lock();
		}
		
		TASK task;

		if (!g_TaskQueue.empty())
		{
			task = g_TaskQueue.front();
			g_TaskQueue.pop();
	
		}

		g_mutex.unlock();

		if(nullptr != task.Functor)
			CALL_FUNC_PAR1(task.Functor, task.pData);

		g_mutex.lock();

		g_iRunningThread++;
		if (g_iRunningThread == g_iTaskCnt)
		{
			g_MainCV.notify_one();
		}
		g_mutex.unlock();
	}

}

bool CThreadPool::RunMain(void)
{
	g_mutex.lock();
	if (g_TaskQueue.empty())
	{
		g_mutex.unlock();
		return false;
	}

	TASK task = g_TaskQueue.front();
	g_TaskQueue.pop();

	g_mutex.unlock();

	g_iRunningThread++;
	if (nullptr != task.Functor)
		CALL_FUNC_PAR1(task.Functor, task.pData);

	return true;
}

void CThreadPool::Push(TASK tTask)
{
	g_iTaskCnt++;
	g_TaskQueue.push(tTask);
}

void CThreadPool::WakeThreads(void)
{
	g_iRunningThread = 0;
	g_WorkerCV.notify_all();

}

void CThreadPool::MainWait(void)
{
	std::unique_lock<std::mutex> LockMain(g_mutex);
	if (g_iRunningThread != g_iTaskCnt)
	{
		g_MainCV.wait(LockMain);
	}

}

bool CThreadPool::CheckNowRunning(void)
{
	return g_iRunningThread != 0;
}
