#include <unistd.h>
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
	pthread_mutex_init(&m_pInternal->mutex, NULL);
}

CMutex::~CMutex()
{
	pthread_mutex_destroy(&m_pInternal->mutex);
	delete m_pInternal;
	m_pInternal = NULL;
}

bool CMutex::lock()
{
	return pthread_mutex_lock(&m_pInternal->mutex) == 0 ? true : false;
}

bool CMutex::trylock()
{
	return pthread_mutex_trylock(&m_pInternal->mutex) == 0 ? true : false;
}

bool CMutex::unlock()
{
	return pthread_mutex_unlock(&m_pInternal->mutex) == 0 ? true : false;
}

struct ThreadInternal
{
	pthread_t handle;
	Infra::CMutex mutex;	
	bool bLoop;				//标示线程执行体是否循环,用户可以设置
	bool isTreadBodyEnd;	//标示线程执行体是否执行，只能有ThreadInternal::proc设置
	bool isRuning;			//标示线程是否运行,只能有CThread::create和CThread::destory可以设置
	CThread* owner;
	static void* proc(void* arg);
};

void* ThreadInternal::proc(void* arg)
{
	ThreadInternal* pInternal = (ThreadInternal*)arg;
	
	pInternal->mutex.lock();
	pInternal->bLoop = true;
	pInternal->mutex.unlock();

	pInternal->owner->thread_proc();
	return NULL;
}


CThread::CThread()
{
	m_pInternal = new ThreadInternal();
	m_pInternal->isRuning = false;
	m_pInternal->bLoop = false;
	m_pInternal->owner = this;
}

CThread::~CThread()
{
	destroy();
	
	delete m_pInternal;
	m_pInternal = NULL;
}

bool CThread::loop() const
{
	return m_pInternal->bLoop;
}

void CThread::run()
{
	create();
}

bool CThread::isTreadRuning() const
{
	return m_pInternal->isRuning;
}


bool CThread::create()
{
	m_pInternal->mutex.lock();
	if (m_pInternal->isRuning)
	{
		m_pInternal->mutex.unlock();
		//线程已经运行
		return false;
	}

	if(!m_pInternal->bLoop)
	{
		while (m_pInternal->isRuning)
		{
			//此时说明线程真正退出,等待线程退出
			m_pInternal->mutex.unlock();
			sleep(1); //TODO:考虑更短延时
			m_pInternal->mutex.lock();
		}
	}
	
	int ret = pthread_create(&m_pInternal->handle, NULL, (void*(*)(void*))&ThreadInternal::proc, (void*)m_pInternal);
	
	m_pInternal->isRuning =	true;
	
	if (ret)
	{
		//线程创建失败
		m_pInternal->isRuning =	false;
		m_pInternal->mutex.unlock();
		printf("create pthread error: %d \n", ret);
		return false;
	}
	else
	{
		m_pInternal->mutex.unlock();
		return true;
	}

	//TODO:分离线程
}

void CThread::destroy()
{
	m_pInternal->mutex.lock();
	if (m_pInternal->isRuning)
	{
		
		pthread_join(m_pInternal->handle, NULL);
		m_pInternal->bLoop = false;
		m_pInternal->isRuning = true;
	}
	else
	{
		/* code */
		
	}
	m_pInternal->mutex.unlock();
}

}//Infra
