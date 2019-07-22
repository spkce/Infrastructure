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
	inline CGuard(T & lock):m_lock(lock)
	{
		m_lock.lock();
	}
	inline ~CGuard()
	{
		m_lock.unlock();
	}
private:
	T & m_lock;
};

struct CondInternal;

class CCondSignal
{
public:
	CCondSignal();
	virtual ~CCondSignal();
	bool wait();
	bool signal();
private:
	struct CondInternal* m_pInternal;
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
	bool loop() const;
	void run();
private:
	bool create();
	void destroy();
	bool isThreadOver();
	struct ThreadInternal* m_pInternal;
};
} //Infra

#endif //__THREAD_H__
