#ifndef __TIMER_H__
#define __TIMER_H__

#include "TFuncation.h"

namespace Infra
{

struct TimerInternal;

class CTimer
{
public:
	typedef TFuncation1<void, int> TimerProc_t;
public:
	CTimer(const char* name);
	virtual ~CTimer();
	
	bool setTimerAttr(TimerProc_t & proc, unsigned int period, unsigned int delay = 0, int times = -1);
	bool setProc(TimerProc_t & proc);
	bool run();
	
private:
	TimerInternal* m_pInternal;
};


}//Infra

#endif //__TIMER_H__
