#ifndef __TIMER_H__
#define __TIMER_H__

namespace Infra
{

struct TimerInternal;

class CTimer
{
public:
	CTimer();
	virtual ~CTimer();
private:
	TimerInternal* m_internal;
};


}//Infra

#endif //__TIMER_H__
