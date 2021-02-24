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
		int i = 0;
		
		while(i < 14)
		{
			if ( i == 5)
			{
				m_cond.wait();
			}
			printf("CThreadTest::thread_proc: %d\n", i++);
			sleep(1);
		}
		
	}

};

void thread_test(void)
{
	int i = 10;
	CThreadTest a;
	a.start();
	while(i--)
	{
		sleep(1);
		printf("thread_test:%d \n",i);
	}
	m_cond.signal();
	while(1);

	printf("thread_test retrun \n");
}

