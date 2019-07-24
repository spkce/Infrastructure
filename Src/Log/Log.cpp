#include "stdio.h"
#include "execinfo.h"
#include "Log.h"
#include <stdarg.h>

void print_backtrace()
{
	void * arry[10] = {0};

	size_t size = backtrace(arry, sizeof(arry));

	for (size_t i = 1; i < size; i++)
	{
		if (arry[i]) printf("%p \n", arry[i]);
	}
}

void printd(const char *fmt, ...)
{
	int ch;
	va_list arg;
	va_start(arg, fmt);
	while (*fmt != '\0')
	{
		switch (*fmt)
		{
			case '%':            //遇到%执行switch case语句
			{
				if (*(fmt + 1) == 'c')//输出字符
				{
					ch = va_arg(arg, char);
					putchar(ch);
					fmt++;                //指针变量向下偏移一个单位
				}
				else if (*(fmt + 1) == 'd')
				{
					ch = va_arg(arg, char);//输出×××
					putchar(ch);
					fmt++;
				}
				else if (*(fmt + 1) == 's')//输出字符串
				{
					char*p = va_arg(arg, char*);
					while (*p != '\0')
					{
						putchar(*p);
						p++;
					}
					fmt++;                //指向头一变量的下一个字符
				}
				else
				{
					putchar('%');
				}
			}
				break;
			default:
				putchar(*fmt);
				break;
		}
		fmt++;
	}
	va_end(arg);
}

inline void setColor(int fontColor, int backgroundColor)
{
	fprintf(stdout,"\033[%d;%dm", fontColor, backgroundColor);
}

inline void resetColor()
{
	fprintf(stdout,"\033[0m");
}

void print_log(const char* fmt, int fontColor = Font_white, int backgroundColor = background_black, ...);
{
	setColor(fontColor, backgroundColor);
	resetColor();
}
