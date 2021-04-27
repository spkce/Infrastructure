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
typedef TFuncation1<void, void *> ThreadProc_t;
/**
* @brief 线程基类
**/
class IThread
{
friend struct ThreadInternal;
protected:
	IThread();
	virtual ~IThread();
	int create(struct ThreadInternal* pInternal);
	struct ThreadInternal* allocateThread();
	void releaseThread(struct ThreadInternal* pInternal);
	
	ThreadProc_t m_proc;
	struct ThreadInternal* m_pInternal;
};

/**
* @brief 线程类
**/
class CThread : public IThread
{
public:
	CThread();
	virtual ~CThread();
	/**
	* @brief 创建线程，创建后线程处于挂起状态
	* @param isBlock 是否以阻塞的方式创建线程。
	* @return true:成功；false:失败
	**/
	bool createTread(bool isBlock = false);

	/**
	* @brief 线程开始运行
	* @param isLoop 是否只循环运行。
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

	/**
	* @brief 注册线程执行函数
	* @param proc 执行函数
	* @return true:成功；false:失败
	**/
	bool attachProc(const ThreadProc_t & proc);

	/**
	* @brief 注册线程执行函数
	* @param proc 已经注册过的执行函数
	* @return true:成功；false:失败
	**/
	bool detachProc(const ThreadProc_t & proc);

	/**
	* @brief 线程是否已经成功创建（createTread）
	* @return true:已创建；false:未创建
	**/
	bool isTreadCreated() const;
};

} //Infra

#endif //__THREAD_H__
