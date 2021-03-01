#include "stdio.h"
#include <unistd.h>
#include "thread.h"

Infra::CCondSignal m_cond;

class CThreadTest : public Infra::CThread
{
public:
	CThreadTest():CThread()
	{

	}
	void start()
	{
		run(true);
	}

protected:
	
	void thread_proc()
	{
		static int i = 0;
		if ( i == 5)
		{
			//m_cond.wait();
		}
		printf("CThreadTest::thread_proc: %d\n", i++);
		sleep(1);
		
	}

};

void thread_test(void)
{
	int i = 10;
	CThreadTest a;
	a.createTread(false);
	a.start();
	while(i--)
	{
		sleep(1);
		if (i == 5)
		{
			a.suspend();
		}
		if (i == 1)
		{
			a.pasue();
		}
		printf("thread_test:%d \n",i);
	}
	m_cond.signal();


	printf("thread_test retrun \n");
}

