
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
	Infra::CMutex mutex;
	CTimer::TimerProc_t proc;
	int times;
	unsigned int delay;
	unsigned int period;
	bool isIdle;
	char name[32];
	
};

TimerInternal::TimerInternal()
:mutex()
,proc()
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
private:
	CLink m_linkEmployTimer;
	CLink m_linkIdleTimer;
	
	unsigned int m_iWorkTimer;
	unsigned int m_iIdleTimer;

};

CTimerManger::CTimerManger()
:m_linkEmployTimer()
,m_linkIdleTimer()
,m_iWorkTimer(0)
,m_iIdleTimer(PER_TIMER_ALLOCATE)
{
	allocateIdleTimer(m_iIdleTimer);

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
	unsigned int iEmployLink = m_linkEmployTimer.linkSize();

	if (iEmployLink == 0)
	{
		goto INSERT_TIMER;
	}

	for (i = 0; i < iEmployLink; i++)
	{
		pTemp = (TimerInternal*)m_linkEmployTimer.get(i);
		if (pTemp == NULL)
		{
			m_linkEmployTimer.rise((void*)p);
			m_iWorkTimer++;
			return ;
		}

		if (pTemp->delay == 0)
		{
			if (pTemp->period > iTemp)
			{
				goto INSERT_TIMER;
			}
		}
		else
		{
			if (pTemp->delay > iTemp)
			{
				goto INSERT_TIMER;
			}
		}
	}

INSERT_TIMER:
	p->isIdle = false;
	m_linkEmployTimer.insert((void*)p, i);
	m_iWorkTimer++;
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
	while(loop())
	{
		long ms = CTime::getSystemTimeNSecond() / 1000;
	}
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
