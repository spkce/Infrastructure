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
	struct MutexInternal* m_pInternal;
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
};

struct ThreadInternal;

class CThread 
{
public:

protected:
	CThread();
	virtual ~CThread();
	virtual void* thread_proc(void *arg) = 0;
	void run();
	bool m_bLoop;
private:
	bool create();
	void destroy();
	struct ThreadInternal* m_pInternal;
};
} //Infra

#endif //__THREAD_H__
