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

struct CondInternal
{
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

CCondSignal::CCondSignal()
{
	m_pInternal = new CondInternal();
	pthread_mutex_init(&m_pInternal->mutex, NULL);
	pthread_cond_init(&m_pInternal->cond, NULL);
}

CCondSignal::~CCondSignal()
{
	pthread_cond_destroy(&m_pInternal->cond);
	pthread_mutex_destroy(&m_pInternal->mutex);
	delete m_pInternal;
	m_pInternal = NULL;
}

bool CCondSignal::wait()
{
	pthread_mutex_lock(&m_pInternal->mutex);
	int ret = pthread_cond_wait(&m_pInternal->cond, &m_pInternal->mutex);
	pthread_mutex_unlock(&m_pInternal->mutex);

	return ret == 0 ? true : false;
}


bool CCondSignal::signal()
{
	return pthread_cond_signal(&m_pInternal->cond) == 0 ? true : false;
}


struct ThreadInternal
{
	pthread_t handle;
	Infra::CMutex mutex;
	//pthread_cond_t cond;	//用于阻塞方式退出
	bool bLoop;				//标示线程执行体是否循环,用户可以设置
	//bool isTreadBodyEnd;	//标示线程执行体是否执行，只能有ThreadInternal::proc设置
	bool isRuning;			//标示线程是否运行,只能有CThread::create和CThread::destory可以设置
	bool isDestoryBlock;	//标示线程退出时是否以阻塞方式退出
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

	pInternal->bLoop = false;
	
	return NULL;
}


CThread::CThread()
{
	m_pInternal = new ThreadInternal();
	m_pInternal->isRuning = false;
	m_pInternal->bLoop = false;
	m_pInternal->owner = this;
	m_pInternal->isDestoryBlock = true;
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
	//TODO:设置线程参数
	int ret;
	ret = pthread_create(&m_pInternal->handle, NULL, (void*(*)(void*))&ThreadInternal::proc, (void*)m_pInternal);
	if (ret)
	{
		//线程创建失败
		m_pInternal->isRuning =	false;
		m_pInternal->mutex.unlock();
		printf("create pthread error: %d \n", ret);
		return false;
	}

	ret = pthread_detach(m_pInternal->handle);
	if (ret)
	{
		printf("detach pthread error: %d \n", ret);
	}

	m_pInternal->isRuning = true;
	m_pInternal->mutex.unlock();
	
	return true;
}

void CThread::destroy()
{
	m_pInternal->mutex.lock();

	if (isThreadOver())
	{
		//线程没有运行
		m_pInternal->mutex.unlock();
		return ;
	}

	pthread_t curTID = pthread_self(); 
	if (pthread_equal(curTID, m_pInternal->handle))
	{
		//自己关闭自己
		m_pInternal->bLoop = false;
		m_pInternal->mutex.unlock();
		return ;
	}

	if (m_pInternal->isRuning)
	{
		m_pInternal->bLoop = false;
		if (m_pInternal->isDestoryBlock)
		{
			//使用条件变量
			pthread_join(m_pInternal->handle, NULL);
		}
		
		m_pInternal->isRuning = false;
	}

	m_pInternal->mutex.unlock();
}

bool CThread::isThreadOver()
{
	return !m_pInternal->isRuning && !m_pInternal->bLoop;
}


}//Infra
