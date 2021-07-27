#include "stdio.h"
#include <unistd.h>
#include "threadPool.h"


class CThreadPoolTest1
{
	Infra::CPoolThread m_thread;

public:
	CThreadPoolTest1()
	{
		m_thread.attach(Infra::CPoolThread::ThreadProc_t(&CThreadPoolTest1::thread_proc, this));
	}
	~CThreadPoolTest1()
	{
		m_thread.detach(Infra::CPoolThread::ThreadProc_t(&CThreadPoolTest1::thread_proc, this));
	}
	void start()
	{
		m_thread.run(true);
	}
	void thread_proc(void* a)
	{
		printf("\033[35m""CThreadPoolTest1 :: thread_proc""\033[0m\n");
		sleep(1);
	}
};



void threadPool_test(void)
{
	CThreadPoolTest1 test1;

	test1.start();


	sleep(30);

}