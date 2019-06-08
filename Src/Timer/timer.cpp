
#include <string.h>
#include "singlenton.h"
#include "link.h"
#include "timer.h"


namespace Infra
{

struct TimerInternal
{
	
	TimerInternal(const char* name);
	~TimerInternal();
	Infra::CMutex mutex;
	unsigned int iSetTime;
	char name[32];
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
	TimerInternal* getTimerInternal();
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
,m_iIdleTimer(10)
{
	//创建10个空白定时器，放入空闲队列
	TimerInternal* p = new TimerInternal[10];
	for (unsigned int i; i < m_iTotalTimer)
	{
		m_linkIdleTimer.rise((void*)p);
	}
}

CTimerManger::~CTimerManger()
{

}

TimerInternal* CTimerManger::getTimerInternal()
{

}

CTimer::CTimer(const char* name)
{

}

CTimer::~CTimer()
{
}

} //Infra
