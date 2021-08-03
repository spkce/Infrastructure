#include "stdio.h"
#include <unistd.h>
#include "threadPool.h"


class CThreadPoolTest1
{
	Infra::CPoolThread m_thread;

public:
	CThreadPoolTest1()
	{
		bool ret = m_thread.attach(Infra::CPoolThread::ThreadProc_t(&CThreadPoolTest1::thread_proc, this));
		printf("\033[35m""attach : %s""\033[0m\n", ret ? "true" : "false");
	}
	~CThreadPoolTest1()
	{
		m_thread.detach(Infra::CPoolThread::ThreadProc_t(&CThreadPoolTest1::thread_proc, this));
	}
	void start()
	{
		m_thread.run();
	}
	void stop(bool isBlock)
	{
		m_thread.stop(isBlock);
	}
	void thread_proc(void* a)
	{
		printf("\033[35m""CThreadPoolTest1 :: thread_proc""\033[0m\n");
		sleep(1);
	}
};



void threadPool_test(void)
{
	{
		CThreadPoolTest1 test1;

		printf("\033[35m""test1 run ""\033[0m\n");
		test1.start();
		sleep(10);
		printf("\033[35m""test1 stop ""\033[0m\n");
		test1.stop(true);
	}
	sleep(30);

}