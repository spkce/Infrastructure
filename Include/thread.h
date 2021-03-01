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
	/**
	* @brief �����̣߳��������̴߳��ڹ���״̬
	* @param isBlock �Ƿ��������ķ�ʽ�����̡߳�
	* @return true:�ɹ���false:ʧ��
	**/
	bool createTread(bool isBlock = false);

	/**
	* @brief �߳̿�ʼ����
	* @param isLoop �Ƿ�ֻ����һ�Ρ�
	**/
	void run(bool isLoop = true);

	/**
	* @brief �̹߳���
	* @param isBlock �Ƿ�ȴ��̹߳����ŷ���
	**/
	void suspend(bool isBlock = false);

	/**
	* @brief �����̼߳�������
	**/
	void pasue();

	/**
	* @brief �߳���ֹ����
	* @param isBlock �Ƿ�ȴ��߳��˳���ŷ���
	* @return true:�ɹ���false:ʧ��
	**/
	bool stop(bool isBlock = false);
	bool isTreadCreated() const;
	bool loop() const;
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
