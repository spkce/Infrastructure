#include "stdio.h"
#include "timer.h"
#include "ctime.h"
class CTimerTest
{
public:
	CTimerTest():m_timer1("test1"),m_timer2("test2")
	{

	}

	void test()
	{
		printf("setup timer1 \n");
		Infra::CTimer::TimerProc_t callback1(&CTimerTest::timer_proc1, this);
		m_timer1.setTimerAttr(callback1, 1000);
		m_timer1.run();
		printf("setup timer2 \n");
		Infra::CTimer::TimerProc_t callback2(&CTimerTest::timer_proc2, this);
		m_timer2.setTimerAttr(callback2, 2000);
		m_timer2.run();
	}

	void timer_proc1(int t)
	{
		long lTime = Infra::CTime::getRealTimeSecond();
		printf("timer_proc1 time = %d\n", lTime);
	}
	void timer_proc2(int t)
	{
		long lTime = Infra::CTime::getRealTimeSecond();
		printf("timer_proc2 time = %d\n", lTime);
	}
	Infra::CTimer m_timer1;
	Infra::CTimer m_timer2;
};

void timer_test(void)
{
	CTimerTest timer;
	timer.test();
	while(1);
}