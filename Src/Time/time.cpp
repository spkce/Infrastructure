
#include "stdio.h"
#include "time.h"

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

}//Infra
