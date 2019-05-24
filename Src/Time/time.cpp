

#include "time.h"

const unsigned char g_day_per_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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

	unsigned int day = timeStamp/86400
}

CData::CData(CData &data)
{

}

CData::~CData()
{

}