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
	bool create();
	void run(bool isLoop = true);
	void suspend();
	void pasue();
	void stop();
	bool isTreadCreated() const;
	bool loop() const;
public:
	virtual void thread_proc() = 0;
private:
	struct ThreadInternal* m_pInternal;
};

class CComThread
{
public:
	CComThread();
	virtual ~CComThread();

};

} //Infra

#endif //__THREAD_H__
