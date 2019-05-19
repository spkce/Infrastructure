
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
	pthread_t tid;
	bool bLoop;
	bool isRuning;
};

typedef void* (*Callback)(void*);

CThread::CThread()
{
	m_pInternal = new ThreadInternal();
	m_pInternal->isRuning = false;
	m_pInternal->bLoop = false;
}

CThread::~CThread()
{
	destroy();
	
	delete m_pInternal;
	m_pInternal = NULL;
}

void CThread::run()
{
	create();
}

bool CThread::isTreadRuning() const
{
	return m_pInternal->isRuning;
}

void* CThread::proc(void* arg)
{
	printf("CThread::proc \n");
	m_pInternal->isRuning = true;
	while (1)
	{
		printf("m_pInternal->bLoop \n");
		thread_proc(arg);
	}
	m_pInternal->isRuning = false;
	return NULL;
}

bool CThread::create()
{
	if (m_pInternal->isRuning)
	{
		//线程已经运行
		return false;
	}
	printf("create pthread error: %p \n", &CThread::proc);
	int ret = pthread_create(&(m_pInternal->tid), NULL, (Callback)&CThread::proc, NULL);
	if (ret)
	{
		printf("create pthread error: %d \n",ret);
		return false;
	}
	else
	{
		return true;
	}
}

void CThread::destroy()
{
	if (m_pInternal->isRuning)
	{
		m_pInternal->bLoop = false;
		pthread_join(m_pInternal->tid, NULL);
	}
}

}//Infra
