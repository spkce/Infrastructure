
#include <string.h>

//#include "singlenton.h"
#include "link.h"
#include "ctime.h"
#include "timer.h"
#include "thread.h"

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

class CTimerManger: public CThread
{
#define PER_TIMER_ALLOCATE 10
public:
	static CTimerManger* instance()
	{
		static CTimerManger* pInstance = NULL;
		if (pInstance == NULL)
		{
			static Infra::CMutex sm_mutex;
			Infra::CGuard<Infra::CMutex> guard(&sm_mutex);
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
	void thread_proc();
	static long getCurTime();

private:
	CLink m_linkWorkTimer;
	CLink m_linkIdleTimer;
	
	unsigned int m_iWorkTimer;
	unsigned int m_iIdleTimer;

	CMutex m_mutexWorkLink;

	long m_curTime;
};

CTimerManger::CTimerManger()
:m_linkWorkTimer()
,m_linkIdleTimer()
,m_iWorkTimer(0)
,m_iIdleTimer(PER_TIMER_ALLOCATE)
,m_mutexWorkLink()
{
	allocateIdleTimer(m_iIdleTimer);

	m_curTime = getCurTime();
	run();
}

CTimerManger::~CTimerManger()
{

}

TimerInternal* CTimerManger::allocateTimer()
{
	TimerInternal* p = NULL;

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
	unsigned int iEmployLink = m_linkWorkTimer.linkSize();

	if (iEmployLink == 0)
	{
		p->isIdle = false;
		p->setupTime = m_curTime;
		m_linkWorkTimer.insert((void*)p, i);
		m_iWorkTimer++;
		return;
	}
	
	Infra::CGuard<Infra::CMutex> guard(&m_mutexWorkLink);

	for (i = 0; i < iEmployLink; i++)
	{
		pTemp = (TimerInternal*)m_linkWorkTimer.get(i);
		if (pTemp == NULL)
		{
			p->isIdle = false;
			p->setupTime = m_curTime;
			m_linkWorkTimer.rise((void*)p);
			m_iWorkTimer++;
			return;
		}
		
		if ((unsigned int)(pTemp->getTimeout() - m_curTime) > iTemp)
		{
			p->isIdle = false;
			p->setupTime = m_curTime;
			m_linkWorkTimer.insert((void*)p, i);
			m_iWorkTimer++;
			return;
		}
	}
}

void CTimerManger::allocateIdleTimer(unsigned int n)
{
	//创建10个空白定时器，放入空闲队列
	TimerInternal* p = new TimerInternal[n];
	for (unsigned int i = 0; i < n; i++)
	{
		m_linkIdleTimer.rise((void*)p);
		m_iIdleTimer++;
	}
}

void CTimerManger::thread_proc()
{
	TimerInternal* p = NULL;

	while(loop())
	{
		p = (TimerInternal*)m_linkWorkTimer.get(0);

		m_curTime = getCurTime();

		if (p->getTimeout() <= m_curTime)
		{
			m_mutexWorkLink.lock();
			m_linkWorkTimer.remove((void**)&p, 0);
			m_iWorkTimer--;
			m_mutexWorkLink.unlock();
			p->proc((int)m_curTime);
			
			//重新插入
			setupTimer(p);
		}
		
	}
}

long CTimerManger::getCurTime()
{
	return CTime::getSystemTimeNSecond() / 1000;
}

CTimer::CTimer(const char* name)
{
	m_pInternal = CTimerManger::instance()->allocateTimer();

	strncpy(m_pInternal->name, name, sizeof(name) -1 );
}

CTimer::~CTimer()
{
}

bool CTimer::setTimerAttr(TimerProc_t & proc, unsigned int period, unsigned int delay, int times)
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

bool CTimer::setProc(TimerProc_t & proc)
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
	if (m_pInternal == NULL)
	{
		return false;
	}
	
	CTimerManger::instance()->setupTimer(m_pInternal);
	return true;
}

} //Infra
