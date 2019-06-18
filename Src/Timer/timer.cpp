
#include <string.h>
#include "singlenton.h"
#include "link.h"
#include "timer.h"


namespace Infra
{

struct TimerInternal
{
	
	TimerInternal();
	~TimerInternal();
	Infra::CMutex mutex;
	char name[32];
	unsigned int delay;
	unsigned int period;
};

TimerInternal::TimerInternal(const char* name)
{
	memset(name, 0, sizeof(name));
	iSetTime = 0;
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
	void allocateIdleTimer(unsigned int n)
private:
	CLink m_linkEmployTimer;
	CLink m_linkIdleTimer;
	
	unsigned int m_iTotalTimer;
	unsigned int m_iIdleTimer;

};

CTimerManger::CTimerManger()
:m_linkEmployTimer()
,m_linkIdleTimer()
,m_iTotalTimer(10)
,m_iIdleTimer(PER_TIMER_ALLOCATE)
{
	allocateIdleTimer(m_iIdleTimer);
}

CTimerManger::~CTimerManger()
{

}

TimerInternal* CTimerManger::allocateTimer()
{
	TimerInternal* P = NULL;

	if (m_linkIdleTimer == 0)
	{
		allocateIdleTimer(PER_TIMER_ALLOCATE);
	}

	m_linkIdleTimer.reduce((void**)&p);
	m_iIdleTimer--;
	m_iTotalTimer--;
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
			m_iTotalTimer++
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
	m_linkEmployTimer.insert((void*)p, i);
	m_iTotalTimer++;
}

void CTimerManger::allocateIdleTimer(unsigned int n)
{
	//创建10个空白定时器，放入空闲队列
	TimerInternal* p = new TimerInternal[n];
	for (unsigned int i = 0; i < n; i++)
	{
		m_linkIdleTimer.rise((void*)p);
		m_iIdleTimer++;
		m_iTotalTimer++;
	}
}
CTimer::CTimer(const char* name)
{

}

CTimer::~CTimer()
{
}

} //Infra
