#ifndef __TIMER_H__
#define __TIMER_H__

#include "TFuncation.h"

namespace Infra
{

struct TimerInternal;

class CTimer
{
public:
	CTimer(const char* name);
	virtual ~CTimer();
	bool setTime(unsigned int period, unsigned int delay = 0, int times = -1);
	bool run();
	
	typedef TFuncation1<void, int> TimerProc_t;
private:
	TimerInternal* m_pInternal;
};


}//Infra

#endif //__TIMER_H__
