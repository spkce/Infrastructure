#include "stdio.h"
#include <unistd.h>
#include "thread.h"

class CThreadTest : public Infra::CThread
{
public:
	CThreadTest():CThread()
	{

	}
	void start()
	{
		run();
	}
protected:
	
	void thread_proc()
	{
		int i = 0;
		while(i < 100)
		{
			printf("CThreadTest::thread_proc: %d\n", i++);
			sleep(1);
		}
	}

};

void thread_test(void)
{
	int i = 100;
	CThreadTest a;
	a.start();
	while(i--)
	{
		sleep(1);
		printf("thread_test:%d \n",i);
	}
	while(1);
	printf("thread_test retrun \n");
}

