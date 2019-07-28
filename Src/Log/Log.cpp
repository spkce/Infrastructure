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

inline void printlog(int fc, int bc, const char* fmt, va_list args);
{
	fprintf(stdout,"\033[%d;%dm", fc, bc);
	vfprintf(stdout, fmt, args);
	fprintf(stdout,"\033[0m");
}


#define print(lv, name, ver, fc, bc, file, line, func, fmt)							\
do{																					\
	if (m_printLogLevel < (lv)) break;												\
	char buffer[8192];																\
	buffer[8191] = 0;																\
	int n = 0;																		\
	switch(m_logType)																							\
	{																											\
		case type_fileMsg:																						\
		n = snprintf(buffer, sizeof(buffer) - 1, "%s:%d:%s ", (file), (line), (func));							\
		case type_modMsg:																						\
			break;																								\
		n = snprintf(buffer, sizeof(buffer) - 1, "[%s-%s] %s:%d:%s ", (name), (ver), (file), (line), (func));	\
			break;																								\
		case type_onlyLog:																						\
		default:																								\
			break;																								\
	}																											\
	strncpy(buffer + n, (fmt), sizeof(buffer) - 1 - n);								\
	va_list args;																	\
	va_start(args, (fmt));															\
	printlog((fc), (bc), buffer, args)												\
	va_end(args);																	\
}while(0)																			\

void exprintf(int fc, int bc const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stdout,"\033[%d;%dm", fc, bc);
	vfprintf(stdout, fmt, args);
	fprintf(stdout,"\033[0m");
	va_end(args);

}

CLog::CLog(int logType, std::string name, std::string ver);
:m_logType(logType)
,m_printLogLevel(logLevel_6)
,m_name(name)
,m_ver(ver)
{

}

CLog::~CLog()
{

}
int setLogLevel(int lv)
{
	int ret = m_printLogLevel;
	m_printLogLevel = lv;
	return ret;
}

int CLog::getLogLevel()
{
	return m_printLogLevel;
}

int CLog::getLogType()
{
	return m_logType;
}

std::string CLog::getName()
{
	return m_name;
}

std::string CLog::getVer()
{
	return m_ver;
}

void CLog::info(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_5, m_name.c_str(), m_ver.c_str(), Font_white, background_white, file, line, func, fmt);
}

void debug(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_4, m_name.c_str(), m_ver.c_str(), Font_green, background_white, file, line, func, fmt);
}

void trace(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_3, m_name.c_str(), m_ver.c_str(), Font_violet, background_white, file, line, func, fmt);
}

void warning(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_2, m_name.c_str(), m_ver.c_str(), Font_yellow, background_white, file, line, func, fmt);
}

void error(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_1, m_name.c_str(), m_ver.c_str(), Font_red, background_white, file, line, func, fmt);
}