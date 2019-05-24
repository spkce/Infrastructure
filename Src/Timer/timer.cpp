
#include "timer.h"
#include "singlenton.h"

namespace Infra
{

struct TimerInternal
{
	TimerInternal(const char* name);
	~TimerInternal();
	char name[32];
};

TimerInternal::TimerInternal(const char* name)
{
	strncpy(this->name, name, sizeof(name) - 1);
}

TimerInternal::~TimerInternal()
{

}

class CTimerManger: public CThread
{
public:
	PATTERN_SINGLETON_DECLARE(CTimerManger)
private:
	CTimerManger();
	~CTimerManger();
public:

private:

}

PATTERM_SINGLETON_IMPLEMENT(CTimerManger)

CTimerManger::CTimerManger()
{

}

CTimerManger::~CTimerManger()
{

}

CTimer::CTimer(const char* name)
:m_internal(name)
{

}

CTimer::~CTimer()
{
}

} //Infra
