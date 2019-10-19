
#include "stdio.h"
#include "ctime.h"
#include <time.h>
#include <sys/select.h>
#include <errno.h>

namespace Infra
{
const unsigned int g_iDayPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned int g_iDayPerMonthleapyear[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
CData::CData()
:m_year(1970)
,m_month(1)
,m_day(1)
,m_hour(0)
,m_minute(0)
,m_second(0)
,m_timeStamp(0)
{

}

CData::CData(long long timeStamp)
{
	m_second = timeStamp%60;
	m_minute = timeStamp%3600/60;
	m_hour = timeStamp%86400/3600;

	//计算年月
	int ileapYear = timeStamp/86400/(365*4+1);	//计算多少个闰年周期，第一个周期为(1970平、1971平、1972闰、1973平)
	m_year = 1970 + ileapYear * 4;

	
	int day = timeStamp/86400%(365*4+1); //超出闰年周期天数

	int year = 0;
	if (day < 365*2)
	{
		year = day / 365;
		day = day - year * 365;
	}
	else if (day < 365*3 + 1)
	{
		year = 2;
		day = day - 2 * 365;
	}else
	{
		year = 3;
		day = day - 3 * 365 - 1;
	}
	m_year = m_year + year;
	
	int temp = 0;
	if (year == 2)
	{
		for (size_t i = 0; i < 12; i++)
		{
			temp = temp + g_iDayPerMonthleapyear[i];
			if(day <= temp)
			{
				m_day = day - temp + g_iDayPerMonthleapyear[i];
				m_month = i;
				break;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < 12; i++)
		{
			temp = temp + g_iDayPerMonth[i];
			if(day <= temp)
			{
				m_day = day - temp + g_iDayPerMonth[i];
				m_month = i;
				break;
			}
		}
	}
	  
}

CData::CData(CData &data)
{

}

CData::~CData()
{

}
//int clock_gettime(clockid_t clk_id,struct timespec *tp);
//clock_gettime编译时出现未定义错误需要连接librt.so.1库  -lrt
//参数：
//clk_id : 检索和设置的clk_id指定的时钟时间。
//CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,
//中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
//　　CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
//　　CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间
//　　CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间
//struct timespec
//{
//time_t tv_sec; /* 秒*/
//long tv_nsec; /* 纳秒*/
//};
long CTime::getRealTimeSecond()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time.tv_sec;
}


long CTime::getRealTimeMSecond()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time.tv_nsec /1000000 + time.tv_sec* 1000;
}

long CTime::getSystemTimeSecond()
{
	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_sec;
}

long CTime::getSystemTimeMSecond()
{
	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_nsec /1000000 + time.tv_sec* 1000;
}

long CTime::getProcessTimeSecond()
{
	timespec time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time); 
	return time.tv_sec;
}

long CTime::getProcessTimeMSecond()
{
	timespec time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
	return time.tv_nsec /1000000 + time.tv_sec* 1000;
}

long CTime::getThreadTimeSecond()
{
	timespec time;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time);
	return time.tv_sec;
}

long CTime::getThreadTimeMSecond()
{
	timespec time;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time);
	return time.tv_nsec /1000000 + time.tv_sec* 1000;
}
void CTime::delay_ms(unsigned int ms)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = ms;
	int err;
	do
	{
		err = select(0,NULL,NULL,NULL,&tv);
	}while (err<0 && errno==EINTR);
}
}//Infra
