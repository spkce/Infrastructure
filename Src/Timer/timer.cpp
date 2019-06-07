
#include <string.h>
#include "singlenton.h"
#include "timer.h"


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
	PATTERN_SINGLETON_DECLARE(CTimerManger)

};

PATTERM_SINGLETON_IMPLEMENT(CTimerManger)

CTimerManger::CTimerManger()
{

}

CTimerManger::~CTimerManger()
{

}

CTimer::CTimer(const char* name)
{

}

CTimer::~CTimer()
{
}

} //Infra
