#include <stdio.h>


extern void Log_test(void);
extern void thread_test(void);
extern void link_test(void);
extern void callback_test(void);
extern void timer_test(void);

int main()
{
	//printf("***************packet_test*************\n");
	//packet_test();
	//printf("****************Log_test***************\n");
	Log_test();
	//printf("****************thread_test***************\n");
	//thread_test();
	//printf("****************link_test***************\n");
	//link_test();
	printf("**************callback_test***************\n");
	//callback_test();
	printf("**************timer_test***************\n");
	//timer_test();
	
	return 0;
}