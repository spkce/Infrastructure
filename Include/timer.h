#ifndef __TIMER_H__
#define __TIMER_H__

namespace Infra
{

struct TimerInternal;

class CTimer
{
public:
	CTimer(const char* name);
	virtual ~CTimer();
private:
	TimerInternal* m_pInternal;
};


}//Infra

#endif //__TIMER_H__
