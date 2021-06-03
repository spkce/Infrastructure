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
		m_thread.createTread();
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
		m_threadA.createTread();
		m_threadA.run(false);

		m_threadB.attachProc(Infra::ThreadProc_t(&CThreadTest2::thread_procB, this));
		m_threadB.createTread();
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
				ret = RWlock.rLock();//��һ�ζ����ļ���
				printf("\033[35m""%s:%d %s read lock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			else if (i == 5)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.unLock(); //��һ�ζ����Ľ���
				printf("\033[35m""%s:%d %s rw unlock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			else if (i == 9)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.unLock();//�ڶ��ζ����Ľ���
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
				ret = RWlock.rLock();//�ڶ��ζ����ļ���
				printf("\033[35m""%s:%d %s read lock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}
			else if (i == 3)
			{
				printf("\033[35m""%s:%d %s i=%d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__,i);
				ret = RWlock.wLock();//��һ��д���ļ���
				printf("\033[35m""%s:%d %s rw wLock : %d""\033[0m\n",__FILE__, __LINE__, __FUNCTION__, ret);
			}

			sleep(1);
		};
		ret = RWlock.unLock();//��һ��д���Ľ���
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
		m_thread.createTread();
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
	*/
	CThreadTest3 test3;
	sleep(5);
	test3.start();
	sleep(10);
	printf("thread_test retrun\n");
}

