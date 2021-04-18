#ifndef __THREAD_H__
#define __THREAD_H__
#include "TFuncation.h"

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

/**
* @brief 线程基类
**/
class IThread
{
friend struct ThreadInternal;
public:
	typedef TFuncation2<void, void *, unsigned int> TimerProc_t;
protected:
	TimerProc_t m_proc;
};

/**
* @brief 线程类，继承方式
**/
class CThread : public IThread
{
protected:
	CThread();
	virtual ~CThread();
public:
	/**
	* @brief 创建线程，创建后线程处于挂起状态
	* @param isBlock 是否以阻塞的方式创建线程。
	* @return true:成功；false:失败
	**/
	bool createTread(bool isBlock = false);

	/**
	* @brief 线程开始运行
	* @param isLoop 是否只运行一次。
	**/
	void run(bool isLoop = true);

	/**
	* @brief 线程挂起
	* @param isBlock 是否等待线程挂起后才返回
	**/
	void suspend(bool isBlock = false);

	/**
	* @brief 挂起线程继续运行
	**/
	void pasue();

	/**
	* @brief 线程终止运行
	* @param isBlock 是否等待线程退出后才返回
	* @return true:成功；false:失败
	**/
	bool stop(bool isBlock = false);
	bool isTreadCreated() const;
	bool loop() const;
	virtual void thread_proc(void * arg, unsigned int size) = 0;
private:
	struct ThreadInternal* m_pInternal;
};

} //Infra

#endif //__THREAD_H__
