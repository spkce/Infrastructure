#include "stdio.h"
#include <unistd.h>
#include "thread.h"

Infra::CCondSignal m_cond;

class CThreadTest
{
public:
	CThreadTest()
	{
		m_thread.attachProc(Infra::ThreadProc_t(&CThreadTest::thread_proc, this));
		m_thread.createTread();
		m_thread.run(true);
	}
	void thread_proc(void* a)
	{
		static int i = 0;
		if ( i == 5)
		{
			//m_cond.wait();
		}
		printf("%s: %d\n", __FUNCTION__,i++);
		sleep(1);
		
	}
	Infra::CThread m_thread;
};

void thread_test(void)
{
	int i = 10;
	
	CThreadTest b;

	while(i--)
	{
		sleep(1);
		if (i == 5)
		{
			b.m_thread.suspend();
		}
		if (i == 1)
		{
			b.m_thread.pasue();
		}
		printf("thread_test:%d \n",i);
	}
	m_cond.signal();


	printf("thread_test retrun \n");
}

