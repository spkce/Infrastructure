
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
	m_pInternal->isRuning = true;
	while (m_pInternal->bLoop)
	{
		thread_proc(arg);
	}
	m_pInternal->isRuning = false;
}

bool CThread::create()
{
	if (m_pInternal->isRuning)
	{
		//线程已经运行
		return false;
	}
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
		while (m_pInternal->isRuning)
		{
			//TODO: 增加延时
			//sleep(1);
		}
	}
}

}//Infra
