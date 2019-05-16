
#include <pthread.h>
#include "thread.h"
#include "stdio.h"

namespace Infra
{

struct MutexInternal
{
	pthread_mutex_t mutex;
};

CMutex::CMutex()
{
	m_pInternal = new MutexInternal();
	pthread_mutex_init(&(m_pInternal->mutex), NULL);
}

CMutex::~CMutex()
{
	pthread_mutex_destroy(&(m_pInternal->mutex));
	delete m_pInternal;
	m_pInternal = NULL;
}

bool CMutex::lock()
{
	return pthread_mutex_lock(&(m_pInternal->mutex)) == 0 ? true : false;
}

bool CMutex::trylock()
{
	return pthread_mutex_trylock(&(m_pInternal->mutex)) == 0 ? true : false;
}

bool CMutex::unlock()
{
	return pthread_mutex_unlock(&(m_pInternal->mutex)) == 0 ? true : false;
}

struct ThreadInternal
{
//	ThreadInternal();
//	~ThreadInternal();
//	void thread_proc();
//	void (*proc)(void *arg);
	pthread_t tid;
};
//ThreadInternal::ThreadInternal
//:proc(NULL);
//{
//	
//}
//ThreadInternal::~ThreadInternal
//{
//
//}
//void ThreadInternal::thread_proc(void *arg)
//{
//	if (proc != NULL)
//	{
//		*proc(arg);
//	}
//}
typedef void* (*Callback)(void*);
CThread::CThread()
:m_bLoop(false)
{
	m_pInternal = new ThreadInternal();
	//m_pInternal->thread_proc = CThread::thread_proc;
}

CThread::~CThread()
{
	destroy();
}

void CThread::run()
{

}

bool CThread::create()
{
	if (m_bLoop)
	{
		//程序已经运行
		return false;
	}
	int ret = pthread_create(&(m_pInternal->tid), NULL, (Callback)&CThread::thread_proc, NULL);
	if (ret)
	{
		printf("create pthread error!\n");
		return false;
	}
	else
	{
		m_bLoop = true;
		return true;
	}
}
void CThread::destroy()
{
	if (m_bLoop)
	{
		pthread_cancel(m_pInternal->tid);
	}
	delete m_pInternal;
	m_pInternal = NULL;
}

}//Infra
