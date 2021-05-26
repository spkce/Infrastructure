
#include <unistd.h>
#include <string.h>

//#include "singlenton.h"
#include "link.h"
#include "ctime.h"
#include "timer.h"
#include "thread.h"
#include "stdio.h"
#include "logInternal.h"

namespace Infra
{

struct TimerInternal
{
	TimerInternal();
	~TimerInternal();
	inline long getTimeout()
	{
		return setupTime + (delay == 0 ? period : delay);
	}
	Infra::CMutex mutex;
	CTimer::TimerProc_t proc;
	long setupTime;
	int times;
	unsigned int delay;
	unsigned int period;
	bool isIdle;
	char name[32];
	
};

TimerInternal::TimerInternal()
:mutex()
,proc()
,setupTime(0)
,times(-1)
,delay(0)
,period(0)
,isIdle(true)
{
	memset(name, 0, sizeof(name));
}

TimerInternal::~TimerInternal()
{

}

class CTimerManger
{
#define PER_TIMER_ALLOCATE 10
public:
	static CTimerManger* instance()
	{
		static CTimerManger* pInstance = NULL;
		if (pInstance == NULL)
		{
			static Infra::CMutex sm_mutex;
			Infra::CGuard<Infra::CMutex> guard(sm_mutex);
			if (pInstance == NULL)
			{
				pInstance = new CTimerManger;
			}
		}
		return pInstance;
	}
private:
	CTimerManger();
	~CTimerManger();
public:
	TimerInternal* allocateTimer();
	void setupTimer(TimerInternal* p);
private:
	void allocateIdleTimer(unsigned int n);
	void thread_proc(void* arg);
	static long getCurTime();

private:
	CLink m_linkWorkTimer;
	CLink m_linkIdleTimer;
	
	unsigned int m_iWorkTimer;
	unsigned int m_iIdleTimer;

	CMutex m_mutexWorkLink;
	CMutex m_mutexIdleLink;
	CMutex m_mutexCurTime;
	long m_curTime;
	CThread m_thread;
};

CTimerManger::CTimerManger()
:m_linkWorkTimer()
,m_linkIdleTimer()
,m_iWorkTimer(0)
,m_iIdleTimer(PER_TIMER_ALLOCATE)
,m_mutexWorkLink()
,m_mutexIdleLink()
,m_mutexCurTime()
{
	allocateIdleTimer(m_iIdleTimer);

	m_curTime = getCurTime();
	
	m_thread.attachProc(Infra::ThreadProc_t(&CTimerManger::thread_proc, this));
	m_thread.createTread();
	m_thread.run();
}

CTimerManger::~CTimerManger()
{
	m_thread.stop(true);
	m_thread.detachProc(Infra::ThreadProc_t(&CTimerManger::thread_proc, this));
}

TimerInternal* CTimerManger::allocateTimer()
{
	TimerInternal* p = NULL;

	Infra::CGuard<Infra::CMutex> guard(m_mutexIdleLink);
	if (m_linkIdleTimer.linkSize() == 0)
	{
		allocateIdleTimer(PER_TIMER_ALLOCATE);
	}

	m_linkIdleTimer.reduce((void**)&p);
	m_iIdleTimer--;
	return p;
}

void CTimerManger::setupTimer(TimerInternal* p)
{
	TimerInternal* pTemp = NULL;
	unsigned int i = 0;
	unsigned int iTemp = (p->delay !=0) ? p->delay : p->period;
	InfraTrace("setupTimer name: %s \n", p->name);
	Infra::CGuard<Infra::CMutex> guard(m_mutexWorkLink);

	const unsigned int iEmployLink = m_linkWorkTimer.linkSize();

	m_curTime = getCurTime();
	
	InfraTrace("m_curTime = %d ms \n", m_curTime);
	
	if (iEmployLink == 0)
	{
		p->isIdle = false;
		p->setupTime = m_curTime;
		m_linkWorkTimer.insert((void*)p, i);
		InfraTrace("i = #1 %d  \n", i);
		m_iWorkTimer++;
		return;
	}
	InfraTrace("iEmployLink = %d \n", iEmployLink);
	for (i = 0; i < iEmployLink; i++)
	{
		InfraTrace("i = %d \n", i);
		pTemp = (TimerInternal*)m_linkWorkTimer.get(i);
		//此定时器是第一个装载
		if (pTemp == NULL)
		{
			p->isIdle = false;
			p->setupTime = m_curTime;
			m_linkWorkTimer.rise((void*)p);
			printf("i = #2 %d  \n", i);
			m_iWorkTimer++;
			return;
		}
		InfraTrace("pTemp->getTimeout() = %d \n", pTemp->getTimeout());
		InfraTrace("iTemp = %d \n", iTemp);
		//按timeout时间查找位置
		if ((unsigned int)(pTemp->getTimeout() - m_curTime) > iTemp)
		{
			p->isIdle = false;
			p->setupTime = m_curTime;
			m_linkWorkTimer.insert((void*)p, i);
			InfraTrace("i = #3 %d  \n", i);
			m_iWorkTimer++;
			return;
		}
	}

	//此定时器timeout时间最长
	p->isIdle = false;
	p->setupTime = m_curTime;
	m_linkWorkTimer.rise((void*)p);
	InfraTrace("i = #4  \n");
	m_iWorkTimer++;
	return;
}

void CTimerManger::allocateIdleTimer(unsigned int n)
{
	//创建10个空白定时器，放入空闲队列
	TimerInternal* p = new TimerInternal[n];
	for (unsigned int i = 0; i < n; i++)
	{
		m_linkIdleTimer.rise((void*)(p + i));
		m_iIdleTimer++;
	}
}

void CTimerManger::thread_proc(void* arg)
{
	TimerInternal* p = NULL;

	long timeout = 0;
	//while(loop())
	do
	{
		
		m_mutexWorkLink.lock();
		p = (TimerInternal*)m_linkWorkTimer.get(0);
		if (p == NULL)
		{
			m_mutexWorkLink.unlock();
			usleep(1000);
			continue;
		}
		timeout = p->getTimeout();
		m_mutexWorkLink.unlock();

		m_curTime = getCurTime();

		if (timeout <= m_curTime)
		{
			InfraTrace("m_curTime = %d <= Timeout =%d ms \n", m_curTime, timeout);
			m_mutexWorkLink.lock();
			m_linkWorkTimer.remove((void**)&p, 0);
			m_iWorkTimer--;
			m_mutexWorkLink.unlock();
			p->proc((int)m_curTime);
			
			//重新插入
			setupTimer(p);
		}

		usleep(1000);
	} while(0);
}

long CTimerManger::getCurTime()
{
	return CTime::getSystemTimeMSecond();
}

CTimer::CTimer(const char* name)
{
	m_pInternal = CTimerManger::instance()->allocateTimer();

	strncpy(m_pInternal->name, name, sizeof(m_pInternal->name) -1 );
}

CTimer::~CTimer()
{
}

bool CTimer::setTimerAttr(const TimerProc_t & proc, unsigned int period, unsigned int delay, int times)
{
	if (m_pInternal == NULL)
	{
		return false;
	}
	
	m_pInternal->period = period;
	m_pInternal->delay = delay;
	m_pInternal->times = times;
	m_pInternal->proc = proc;
	return true;
}

bool CTimer::setProc(const TimerProc_t & proc)
{
	if (m_pInternal == NULL)
	{
		return false;
	}

	m_pInternal->proc = proc;

	return true;
}

bool CTimer::run()
{
	InfraTrace("CTimer::run \n");
	if (m_pInternal == NULL)
	{
		InfraTrace("m_pInternal == NULL \n");
		return false;
	}
	
	InfraTrace("setup timer : %p \n", m_pInternal);

	CTimerManger::instance()->setupTimer(m_pInternal);
	return true;
}

} //Infra
