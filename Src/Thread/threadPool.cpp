#include "stdio.h"
#include <pthread.h>
#include "thread.h"
#include "threadPool.h"
#include "link.h"
#include "LogInternal.h"
#include "ctime.h"

namespace Infra
{

class CThreadCore
{
public:
enum 
{
	THREAD_INIT,
	THREAD_SUSPEND,
	THREAD_EXCUTE,
	THREAD_WORK,
	THREAD_EXIT,
};

public:
	CThreadCore();
	~CThreadCore();

public:
	int getState() const;
	static void* proc(void* arg);

private:
	bool init();

public:
	//线程句柄
	pthread_t m_handle;
	
	//用于线程的挂起
	CCondSignal m_spndCond;
	//用于通知线程进入和退出
	CCondSignal m_procCond;

	//线程控制标志位，m_rwlock保护，excute、suspend、work中可更改
	CRwlock m_rwlock;
	bool m_bExit;				//线程执行体退出
	bool m_bSuspend;			//线程暂停函数

	//线程拥有者
	void* m_owner;

private:
	//线程状态 由proc函数中设置
	int m_state;
};

CThreadCore::CThreadCore()
:m_handle()
,m_bExit(false)
,m_bSuspend(true)
,m_owner(NULL)
,m_state(THREAD_INIT)
{
	if (init())
	{
		//等待CThreadCore::proc开始执行
		m_procCond.wait();
	}
}

CThreadCore::~CThreadCore()
{
	if (m_handle)
	{
		m_rwlock.wLock();
		m_bExit = true;
		m_rwlock.unLock();
		m_spndCond.signal();

		m_procCond.wait();
	}
}

int CThreadCore::getState() const
{
	return m_state;
}

void* CThreadCore::proc(void* arg)
{
	bool isExit = false;
	bool isSuspend = false;
	CThreadCore* pCore = (CThreadCore*)arg;

	pCore->m_procCond.signal();
	InfraTrace("thread:%p ready\n", pCore);
	pCore->m_state = THREAD_SUSPEND; //默认初始状态为挂起
	pCore->m_spndCond.wait();
	pCore->m_state = THREAD_EXCUTE;
	InfraTrace("thread:%p start\n", pCore);
	do
	{
		pCore->m_rwlock.rLock();
		isExit = pCore->m_bExit;
		isSuspend = pCore->m_bSuspend;
		pCore->m_rwlock.unLock();

		if (isExit)
		{
			break;
		}

		if (isSuspend)
		{
			pCore->m_state = THREAD_SUSPEND;
			pCore->m_rwlock.wLock();
			pCore->m_bSuspend = false;
			pCore->m_rwlock.unLock();
			InfraTrace("thread:%p proc signal\n", pCore);
			pCore->m_spndCond.signal();
			InfraTrace("thread:%p proc suspend\n", pCore);
			pCore->m_spndCond.wait();
			InfraTrace("thread:%p proc suspend end\n", pCore);
			pCore->m_state = THREAD_EXCUTE;

			continue;
		}

		if (pCore->m_owner != NULL)
		{
			pCore->m_state = THREAD_WORK;
			CPoolThread::ThreadProc_t & proc = ((CPoolThread*)(pCore->m_owner))->m_proc;
			if (!proc.isEmpty())
			{
				proc(arg);
			}
			else
			{
				pCore->m_rwlock.wLock();
				pCore->m_bSuspend = true;
				pCore->m_rwlock.unLock();
				continue;
			}
			pCore->m_state = THREAD_EXCUTE;
		}
		else
		{
			InfraTrace("thread:%p Empty\n", pCore);
			pCore->m_rwlock.wLock();
			pCore->m_bSuspend = true;
			pCore->m_rwlock.unLock();
		}
	} while (1);
	
	pCore->m_procCond.signal();
	pCore->m_state = THREAD_EXIT;
	
	InfraTrace("thread:%p proc exit\n", pCore);

	return NULL;
}

bool CThreadCore::init()
{
	int err = pthread_create(&m_handle, NULL, (void*(*)(void*))&CThreadCore::proc, (void*)this);
	if (err)
	{
		//线程创建失败
		InfraTrace("create pthread error: %d\n",err);
		return false;
	}

	err = pthread_detach(m_handle);
	if (err)
	{
		InfraTrace("detach pthread error: %d\n",err);
		return false;
	}
	return true;
}

class CThreadPoolManager
{
public:
	static CThreadPoolManager *instance();

private:
	CThreadPoolManager();
	~CThreadPoolManager();

public:
	CThreadCore *applyCore(void* owner);
	void cancelCore(CThreadCore *core);

private:
	void alloc();
	CMutex m_mutex;
	CLink m_listIdle;
};

CThreadPoolManager* CThreadPoolManager::instance()
{
	static CThreadPoolManager* pInstance = NULL;
	if (pInstance == NULL)
	{
		static CMutex sm_mutex;
		CGuard<CMutex> guard(sm_mutex);
		if (pInstance == NULL)
		{
			pInstance = new CThreadPoolManager;
		}
	}
	return pInstance;
}

CThreadPoolManager::CThreadPoolManager()
{

}

CThreadPoolManager::~CThreadPoolManager()
{

}

CThreadCore * CThreadPoolManager::applyCore(void* owner)
{
	if (m_listIdle.linkSize() == 0)
	{
		alloc();
	}

	CThreadCore* p = NULL;
	CGuard<CMutex> guard(m_mutex);
	m_listIdle.reduce((void**)&p);
	p->m_owner = owner;
	return p;
}

void CThreadPoolManager::cancelCore(CThreadCore *p)
{
	if (p == NULL)
	{
		return ;
	}

	CGuard<CMutex> guard(m_mutex);
	p->m_owner = NULL;
	m_listIdle.rise(p);
}

void CThreadPoolManager::alloc()
{
	void* p = new CThreadCore;
	CGuard<CMutex> guard(m_mutex);
	m_listIdle.rise(p);
}

CPoolThread::CPoolThread()
{
	m_threadCore = CThreadPoolManager::instance()->applyCore((void*)this);
}

CPoolThread::~CPoolThread()
{
	stop(true);
	CThreadPoolManager::instance()->cancelCore(m_threadCore);
}

bool CPoolThread::run()
{
	if (m_proc.isEmpty())
	{
		InfraTrace("thread process funcation empty!\n");
		return false;
	}

	if (m_threadCore->getState() == CThreadCore::THREAD_EXIT)
	{
		return false;
	}

	m_threadCore->m_rwlock.wLock();
	m_threadCore->m_bSuspend = false;
	m_threadCore->m_rwlock.unLock();

	
	if (m_threadCore->getState() == CThreadCore::THREAD_INIT)
	{
		//若m_threadCore刚创建还没有就绪，等待300ms
		CTime::delay_ms(100);
		if (m_threadCore->getState() == CThreadCore::THREAD_INIT)
		{
			return false;
		}
	}

	m_threadCore->m_spndCond.signal();
	return true;
}


bool CPoolThread::stop(bool isBlock)
{
	const int status = m_threadCore->getState();
	if (status == CThreadCore::THREAD_EXCUTE 
		|| status == CThreadCore::THREAD_WORK)
	{
		m_threadCore->m_rwlock.wLock();
		m_threadCore->m_bSuspend = true;
		m_threadCore->m_rwlock.unLock();

		if (isBlock)
		{
			m_threadCore->m_spndCond.wait();
		}
		return true;
	}

	return false;
}

bool CPoolThread::attach(const ThreadProc_t & proc)
{
	const int status = m_threadCore->getState();
	InfraTrace("thread status : %d \n", status);
	if (status == CThreadCore::THREAD_EXCUTE 
		|| status == CThreadCore::THREAD_WORK
		|| status == CThreadCore::THREAD_EXIT)
	{
		return false;
	}

	if (m_proc.isEmpty())
	{
		m_proc = proc;
		return true;
	}

	return false;
}

bool CPoolThread::detach(const ThreadProc_t & proc)
{
	const int status = m_threadCore->getState();
	InfraTrace("thread status : %d \n", status);
	if (status == CThreadCore::THREAD_SUSPEND)
	{
		if (!m_proc.isEmpty())
		{
			m_proc.unbind();
			return true;
		}
	}
	return false;
}

}//Infra
