#include "stdio.h"
#include <string.h>
#include "MsgQueue.h"


void MsgQueue_test(void)
{
	char buf[20] = {0};
	Infra::CMsgQueue queue(5, sizeof(buf));

	snprintf(buf,sizeof(buf),"%d", 1);
	queue.input(buf, sizeof(buf));
	snprintf(buf,sizeof(buf),"%d", 2);
	queue.input(buf, sizeof(buf));
	snprintf(buf,sizeof(buf),"%d", 3);
	queue.input(buf, sizeof(buf));

	memset(buf, 0, sizeof(buf));
	queue.output(buf, sizeof(buf));
	printf("output: %s \n", buf);

	while (1)
	{
		/* code */
	}
	
	memset(buf, 0, sizeof(buf));
	queue.output(buf, sizeof(buf));
	printf("output: %s \n", buf);

	memset(buf, 0, sizeof(buf));
	queue.output(buf, sizeof(buf));
	printf("output: %s \n", buf);
}