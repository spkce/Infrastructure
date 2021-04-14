#include <unistd.h>
#include <pthread.h>
#include "thread.h"
#include "stdio.h"
#include <string.h>
#include <errno.h>
#include "TFuncation.h"

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

enum 
{
	THREAD_INIT,
	THREAD_READY,
	THREAD_SUSPEND,
	THREAD_EXCUTE,
	THREAD_WORK,
	THREAD_EXIT,
};

struct ThreadInternal
{
	pthread_t handle;
	Infra::CMutex mutex;
	Infra::CCondSignal cond;//用于线程的挂起
	bool bLoop;				//线程执行体是否循环,由用户设置
	bool bExit;				//线程执行体退出,由用户设置
	bool bSuspend;			//线程暂停函数,由用户设置true,执行体设置false
	bool isDestoryBlock;	//标示线程退出时是否以阻塞方式退出
	int state;				//线程状态,由proc设置
	CThread* owner;
	static void* proc(void* arg);
};

void* ThreadInternal::proc(void* arg)
{
	bool isLoop = false;
	bool isExit = false;
	bool isSuspend = false;
	ThreadInternal* pInternal = (ThreadInternal*)arg;

	printf("\033[40;35m""%s:%d %s ""\033[0m\n",__FILE__, __LINE__, __FUNCTION__);
	pInternal->cond.signal();
	pInternal->state = THREAD_EXCUTE;
	printf("\033[40;35m""signal""\033[0m\n");
	do
	{
		pInternal->mutex.lock();
		isExit = pInternal->bExit;
		isSuspend = pInternal->bSuspend;
		pInternal->mutex.unlock();
		
		if (isExit)
		{
			break;
		}

		if (isSuspend)
		{
			pInternal->state = THREAD_SUSPEND;
			printf("\033[40;35m""proc wait""\033[0m\n");
			pInternal->cond.signal();
			pInternal->cond.wait();
			pInternal->state = THREAD_EXCUTE;
			continue;
		}
		
		pInternal->state = THREAD_WORK;
		pInternal->owner->thread_proc();
		pInternal->state = THREAD_EXCUTE;

		pInternal->mutex.lock();
		isLoop = pInternal->bLoop;
		pInternal->mutex.unlock();
	} while (isLoop);
	
	pInternal->state = THREAD_EXIT;

	return NULL;
}


CThread::CThread()
{
	m_pInternal = new ThreadInternal();
	m_pInternal->bLoop = false;
	m_pInternal->bExit = false;
	m_pInternal->bSuspend = true;
	m_pInternal->state = THREAD_INIT;
	m_pInternal->owner = this;
	m_pInternal->isDestoryBlock = true;
}

CThread::~CThread()
{
	stop();
	
	delete m_pInternal;
	m_pInternal = NULL;
}

bool CThread::loop() const
{
	return m_pInternal->bLoop;
}

void CThread::run(bool isLoop)
{
	printf("\033[40;35m""%s:%d %s ""\033[0m\n",__FILE__, __LINE__, __FUNCTION__);
	if (m_pInternal->state == THREAD_EXIT)
	{
		return ;
	}
	m_pInternal->mutex.lock();
	m_pInternal->bLoop = isLoop;
	m_pInternal->bSuspend = false;
	m_pInternal->mutex.unlock();

	if (m_pInternal->state == THREAD_SUSPEND)
	{
		m_pInternal->cond.signal();
	}
}

void CThread::suspend(bool isBlock)
{
	if (m_pInternal->state == THREAD_EXCUTE || m_pInternal->state == THREAD_WORK)
	{
		m_pInternal->mutex.lock();
		m_pInternal->bSuspend = true;
		m_pInternal->mutex.unlock();
	}

	if (isBlock)
	{
		m_pInternal->cond.wait();	
	}	
}

void CThread::pasue()
{
	if (m_pInternal->state == THREAD_SUSPEND)
	{
		m_pInternal->mutex.lock();
		m_pInternal->bSuspend = false;
		m_pInternal->mutex.unlock();
		
		m_pInternal->cond.signal();
	}
}

bool CThread::stop(bool isBlock)
{
	if (m_pInternal->state == THREAD_INIT || m_pInternal->state == THREAD_EXIT)
	{
		return false;
	}

	m_pInternal->mutex.lock();
	m_pInternal->bExit = true;
	m_pInternal->bLoop = false;
	m_pInternal->mutex.unlock();

	pthread_t curTID = pthread_self(); 
	if (pthread_equal(curTID, m_pInternal->handle))
	{
		//此函數由线程执行体调用，不等待直接退出
		return true;
	}

	if (m_pInternal->state == THREAD_SUSPEND || m_pInternal->state == THREAD_READY)
	{
		m_pInternal->cond.signal();
	}

	if (isBlock)
	{
		//使用条件变量，等待线程退出
		pthread_join(m_pInternal->handle, NULL);
	}

	return true;
}

bool CThread::isTreadCreated() const
{
	return m_pInternal->state >= THREAD_READY;
}


bool CThread::createTread(bool isBlock)
{
	if (isTreadCreated())
	{
		//线程已经运行
		return false;
	}
	
	//TODO:设置线程参数
	int err;
	err = pthread_create(&m_pInternal->handle, NULL, (void*(*)(void*))&ThreadInternal::proc, (void*)m_pInternal);
	if (err)
	{
		//线程创建失败
		m_pInternal->state = THREAD_EXIT;
		printf("create pthread error: %d \n", err);
		return false;
	}

	err = pthread_detach(m_pInternal->handle);
	if (err)
	{
		printf("detach pthread error: %d \n", err);
	}

	m_pInternal->state = THREAD_READY;
	if (isBlock)
	{
		printf("\033[40;35m""wait""\033[0m\n");
		m_pInternal->cond.wait();
	}

	return true;
}


class CComThread
{
public:
	typedef TFuncation2<void, int, void*> TimerProc_t;
public:
	CComThread();
	virtual ~CComThread();
private:
	struct ThreadInternal* m_pInternal;
};

CComThread::CComThread()
{

}

CComThread::~CComThread()
{

}
}//Infra
