#ifndef __TIME_H__
#define __TIME_H__

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
private:
};

}//Infra

#endif //__TIME_H__
