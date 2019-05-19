#include <stdio.h>


extern void packet_test(void);
extern void Log_test(void);
extern void thread_test(void);

int main()
{
	//printf("***************packet_test*************\n");
	//packet_test();
	//printf("****************Log_test***************\n");
	//Log_test();
	printf("****************thread_test***************\n");
	thread_test();
	return 0;
}