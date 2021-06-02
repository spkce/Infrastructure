#include "stdio.h"
 #include <unistd.h>
#include "timer.h"
#include "ctime.h"

class CTimerTest
{
public:
	CTimerTest():m_timer1("test1"),m_timer2()
	{

	}

	void test()
	{
		printf("setup timer1 \n");
		m_timer1.setTimerAttr(Infra::CTimer::TimerProc_t(&CTimerTest::timer_proc1, this), 1000);
		m_timer1.run();
		printf("setup timer2 \n");
		m_timer2.setTimerAttr(Infra::CTimer::TimerProc_t(&CTimerTest::timer_proc2, this), 2000);
		m_timer2.run();
	}

	void timer_proc1(unsigned long long t)
	{
		unsigned long long lTime = Infra::CTime::getRealTimeSecond();
		printf("timer_proc1 time = %llu\n", lTime);
	}
	void timer_proc2(unsigned long long t)
	{
		unsigned long long lTime = Infra::CTime::getRealTimeSecond();
		printf("timer_proc2 time = %llu\n", lTime);
	}

	void stop(int timer)
	{
		if (timer == 1)
		{
			m_timer1.stop();
		}
		else if (timer == 2)
		{
			m_timer2.stop();
		}
	}
	Infra::CTimer m_timer1;
	Infra::CTimer m_timer2;
};

void timer_test(void)
{
	CTimerTest timer;
	timer.test();
	sleep(10);
	timer.stop(1);
	sleep(5);


}