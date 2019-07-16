#include "stdio.h"
#include "timer.h"

class CTimerTest
{
public:
	CTimerTest():m_timer("test")
	{

	}

	void test()
	{
		Infra::CTimer::TimerProc_t callback(&CTimerTest::timer_proc, this);
		m_timer.setTimerAttr(callback, 1000);
		m_timer.run();
	}

	void timer_proc(int t)
	{
		printf("timer_proc");
	}
	Infra::CTimer m_timer;
};

void timer_test(void)
{
	CTimerTest timer;
	timer.test();
	while(1);
}