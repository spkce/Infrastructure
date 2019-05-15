#ifndef __THREAD_H__
#define __THREAD_H__

namespace Infra
{

struct MutexInternal;

class CMutex
{
public:
	CMutex();
	virtual ~CMutex();
	bool lock();
	bool trylock();
	bool unlock();
private:
	MutexInternal* m_pInternal;
};
template <class T>
class CGuard
{
public:
	inline CGuard(T* lock):m_plock(lock)
	{
		m_plock->lock();
	}
	inline ~CGuard()
	{
		m_plock->unlock();
	}
private:
	T* m_plock;
}

class CThread 
{
public:

protected:
	CThread();
	virtual ~CThread();
	virtual void thread_proc() = 0;
	void run();
private:
	bool create();
	pthread_t m_id;
};
} //Infra

#endif //__THREAD_H__
