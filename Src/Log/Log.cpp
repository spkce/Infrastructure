#include "stdio.h"
#include "execinfo.h"
#include "Log.h"

void print_backtrace()
{
	void * arry[10] = {0};

	size_t size = backtrace(arry, sizeof(arry));

	for (size_t i = 1; i < size; i++)
	{
		if (arry[i]) printf("%p \n", arry[i]);
	}
}