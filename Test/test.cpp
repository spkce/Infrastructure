#include <stdio.h>


extern void packet_test(void);
extern void Log_test(void);
int main()
{
	printf("***************packet_test*************\n");
	packet_test();
	printf("****************Log_test***************\n");
	Log_test();
	return 0;
}