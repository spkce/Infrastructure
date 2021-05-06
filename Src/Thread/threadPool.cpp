#include "stdio.h"
#include "thread.h"
#include "threadPool.h"
#include "link.h"

namespace Infra
{

struct ThreadPoolInternal
{
	CLink listWork;
	CLink listIdle;

	CThread m_workThread;
};

CThreadPoolManager* CThreadPoolManager::instance()
{
	static CThreadPoolManager* pInstance = NULL;
	if (pInstance == NULL)
	{
		static Infra::CMutex sm_mutex;
		Infra::CGuard<Infra::CMutex> guard(sm_mutex);
		if (pInstance == NULL)
		{
			pInstance = new CThreadPoolManager;
		}
	}
	return pInstance;
}

CThreadPoolManager::CThreadPoolManager()
{
	m_pInternal = new ThreadPoolInternal();
	m_pInternal->m_workThread.attachProc(ThreadProc_t(&CThreadPoolManager::workProc, this));
	m_pInternal->m_workThread.run(true);
}

CThreadPoolManager::~CThreadPoolManager()
{

}

IThread* CThreadPoolManager::applyThread()
{

}

bool CThreadPoolManager::cancelThread(IThread* pthread)
{

}

IThread* CThreadPoolManager::allocThread()
{

}

IThread* CThreadPoolManager::releaseThread()
{

}

void CThreadPoolManager::workProc(void* arg)
{

}
}//Infra