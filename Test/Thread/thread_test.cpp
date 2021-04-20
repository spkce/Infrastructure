#include "stdio.h"
#include <unistd.h>
#include "thread.h"

Infra::CCondSignal m_cond;

class CThreadTest
{
public:
	CThreadTest()
	{
		Infra::IThread::ThreadProc_t callback1(&CThreadTest::thread_proc, this);

		m_thread.attachProc(callback1);
		m_thread.run(true);
	}
	void thread_proc(void* a)
	{
		static int i = 0;
		if ( i == 5)
		{
			//m_cond.wait();
		}
		printf("CThreadTest::thread_proc: %d\n", i++);
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

