
#include <pthread.h>
#include "thread.h"

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
	pthread_mutex_destroy(&(m_pInternal->mutex))
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

CThread::CThread()
{
	int ret = 0;

}

CThread::~CThread()
{
	
}

void CThread::run()
{

}

int CThread::create()
{
	int ret = pthread_create(&m_id,NULL,(void*)thread_proc,NULL);
	if (ret)
	{
		printf("create pthread error!\n");
	}
	return ret
}
void CThread::destroy()
{

}
