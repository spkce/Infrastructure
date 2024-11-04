#include "stdio.h"
#include <unistd.h>
#include "thread.h"

Infra::CCondSignal m_cond;


class CThreadTest1
{
public:
	CThreadTest1()
	{
		m_thread.attachProc(Infra::ThreadProc_t(&CThreadTest1::thread_proc, this));
		m_thread.createThread();
		m_thread.run(true);
	}
	
	void thread_proc(void* a)
	{
		static int i = 0;
		if ( i == 5)
		{
			m_cond.wait();
		}
		printf("%s: %d\n", __FUNCTION__,i++);
		sleep(1);
		
	}
	
	Infra::CThread m_thread;
};


class CThreadTest2
{
	Infra::CRwlock RWlock;
	Infra::CThread m_threadA;
	Infra::CThread m_threadB;
public:
	CThreadTest2()
	{
		m_threadA.attachProc(Infra::ThreadProc_t(&CThreadTest2::thread_procA, this));
		m_threadA.createThread();
		m_threadA.run(false);

		m_threadB.attachProc(Infra::ThreadProc_t(&CThreadTest2::thread_procB, this));
		m_threadB.createThread();
		m_threadB.run(false);
	}

	void thread_procA(void* a)
	{
		int i = 0;
		bool ret;
		while (i++ < 10)
		{
			if (i == 1)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.rLock();//????��????????
				printf("\033[35m""%s:%d %s read lock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			else if (i == 5)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.unLock(); //????��????????
				printf("\033[35m""%s:%d %s rw unlock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			else if (i == 9)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.unLock();//????��????????
				printf("\033[35m""%s:%d %s rw unlock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			sleep(1);
		};
	}
	void thread_procB(void* a)
	{
		int i = 0;
		bool ret; 
		while (i++ < 10)
		{
			if (i == 2)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.rLock();//????��????????
				printf("\033[35m""%s:%d %s read lock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			else if (i == 3)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.wLock();//?????��???????
				printf("\033[35m""%s:%d %s rw wLock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}

			sleep(1);
		};
		ret = RWlock.unLock();//?????��???????
		m_cond.signal();
	}
};

class CThreadTest3
{
	Infra::CThread m_thread;
public:
	CThreadTest3()
	{
		m_thread.attachProc(Infra::ThreadProc_t(&CThreadTest3::thread_proc, this));
		m_thread.createThread();
	}
	
	void start()
	{
		m_thread.run();
	}

	void thread_proc(void* a)
	{
		printf("\033[35m""CThreadTest3 :: thread_proc""\033[0m\n");
		sleep(1);
	}
};

class CThreadTest4
{
	Infra::CThread m_thread;
public:
	CThreadTest4()
	{
		m_thread.attachProc(Infra::ThreadProc_t(&CThreadTest4::thread_proc, this));
		m_thread.createThread();
	}
	
	~CThreadTest4()
	{
		m_thread.stop(true);
	}
	void detory()
	{
		printf("CThreadTest4::detory\n");
		delete this;
	}
	void start()
	{
		printf("CThreadTest4::start\n");
		m_thread.run();
	}

	void thread_proc(void* a)
	{
		printf("\033[35m""CThreadTest4 :: thread_proc""\033[0m\n");
		sleep(1);
	}
};


class CThreadTest5
{
	Infra::CThread m_thread;
public:
	CThreadTest5()
	{
		m_thread.attachProc(Infra::ThreadProc_t(&CThreadTest5::thread_proc, this));
	}
	
	~CThreadTest5()
	{
		m_thread.stop(true);
	}
	void start()
	{
		printf("CThreadTest5::start\n");
		m_thread.createThread();
		m_thread.run();
	}
	void stop()
	{
		printf("CThreadTest5::stop wait\n");
		m_thread.stop(true);
		printf("CThreadTest5::stop\n");
	}
	bool isExit()
	{
		return m_thread.isExit();
	}
	void thread_proc(void* a)
	{
		printf("\033[35m""CThreadTest5 :: thread_proc""\033[0m\n");
		sleep(1);
	}
};

void thread_test(void)
{
	/*
	int i = 10;
	
	CThreadTest1 b;

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


	CThreadTest2 test2;
	m_cond.wait();
	
	CThreadTest3 test3;
	sleep(5);
	test3.start();
	sleep(10);
	*/

	//for (int i = 0; i < 10; i++)
	//{
	//	CThreadTest4* p = new CThreadTest4;
	//	p->start();
	//	//sleep(1);
	//	p->detory();
	//}
	//sleep(10);
	//printf("thread_test retrun\n");

	CThreadTest5 test5;

	test5.start();
	sleep(1);
	test5.stop();

	test5.start();


	//for (int i = 0; i < 10; i++)
	//{
	//	test5.start();
	//	sleep(1);
	//	test5.stop();
	//}

	sleep(10);
}

