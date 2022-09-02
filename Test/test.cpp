#include <stdio.h>


extern void Log_test(void);
extern void thread_test(void);
extern void threadPool_test(void);
extern void link_test(void);
extern void callback_test(void);
extern void timer_test(void);
extern void MsgQueue_test(void);
extern void exiter_test(void);

int main()
{
	//printf("***************packet_test*************\n");
	//packet_test();
	//printf("****************Log_test***************\n");
	//Log_test();
	//printf("****************thread_test***************\n");
	//thread_test();
	//printf("****************threadPool_test***************\n");
	//threadPool_test();
	//printf("****************link_test***************\n");
	//link_test();
	
	//callback_test();
	//printf("**************timer_test***************\n");
	//timer_test();
	//printf("**************MsgQueue_test***************\n");
	//MsgQueue_test();
	printf("**************exiter_test***************\n");
	exiter_test();
	return 0;
}

/*
template <typename R, typename P1, typename P2>
class CObServer
{
public:
	typedef Infra::TFuncation2<R, P1, P2> callback_t;

private:
	class CWatcher
	{
	public:
		bool attach(const callback_t);
		bool detach(const callback_t);
		callback_t m_proc;
	}

public:
	bool attach(const callback_t);
	bool detach(const callback_t);

private:
	CWatcher m_watcher;
};
*/