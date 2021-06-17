#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>
namespace Infra
{
//长日期格式
enum
{
	YYYY_MM_DD,
	DD_MM_YYYY,
};

//时间格式
enum
{
	HH_MM_SS,
	HH_MM_SS_APM,
};

class CData
{
public:
	CData();
	CData(long long timeStamp);
	CData(CData &data);
	virtual ~CData();
private:
	unsigned int m_year;
	unsigned int m_month;
	unsigned int m_day;
	unsigned int m_hour;
	unsigned int m_minute;
	unsigned int m_second;
	long long m_timeStamp;
};

class CTime
{
public:
	CTime();
	CTime(CData &t);
	virtual ~CTime();
	//获取当前unix时间戳, 单位:秒
	static unsigned long long getRealTimeSecond();
	//获取当前unix时间戳, 单位:毫秒
	static unsigned long long getRealTimeMSecond();
	//获取系统运行时间, 单位:秒
	static unsigned long long getSystemTimeSecond();
	//获取系统运行时间, 单位:纳秒
	static unsigned long long getSystemTimeMSecond();
	//获取进程运行时间, 单位:秒
	static unsigned long long getProcessTimeSecond();
	//获取进程运行时间, 单位:纳秒
	static unsigned long long getProcessTimeMSecond();
	//获取线程运行时间, 单位:秒
	static unsigned long long getThreadTimeSecond();
	//获取线程运行时间, 单位:纳秒
	static unsigned long long getThreadTimeMSecond();

	//将时间转换成系统实时时间，当前系统实时时间增加ms时间 
	static void covertRealTime(unsigned int ms, timespec *tp);

	static void delay_ms(unsigned int ms);
private:
};

}//Infra

#endif //__TIME_H__
