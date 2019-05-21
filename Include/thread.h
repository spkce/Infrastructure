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
protected:
	CThread();
	virtual ~CThread();
public:
	bool isTreadRuning() const;
	virtual void thread_proc() = 0;
protected:
	void run();
private:
	bool create();
	void destroy();
	struct ThreadInternal* m_pInternal;
};
} //Infra

#endif //__THREAD_H__
