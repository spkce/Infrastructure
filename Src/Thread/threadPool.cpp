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
	m_pInternal->m_workThread.stop(true);
	m_pInternal->m_workThread.detachProc(ThreadProc_t(&CThreadPoolManager::workProc, this));
	delete m_pInternal;
}

IThread* CThreadPoolManager::applyThread()
{
	IThread* p = NULL;
	m_pInternal->listIdle.reduce((void**)&p);
	m_pInternal->listWork.rise((void*)p);
	return p;
}

bool CThreadPoolManager::cancelThread(IThread* pthread)
{
	//m_pInternal->listWork.find()
}

void CThreadPoolManager::allocThread()
{
	IThread* p = new CThread();
	m_pInternal->listIdle.rise((void*)p);
}

void CThreadPoolManager::releaseThread(IThread* pThread)
{
	delete pThread;
}

void CThreadPoolManager::workProc(void* arg)
{

}
}//Infra