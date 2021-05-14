#include "stdio.h"
#include <string.h>
#include <map>
#include "execinfo.h"
#include "Log.h"
#include <stdarg.h>
#include "thread.h"

void print_backtrace()
{
	void * arry[10] = {0};

	size_t size = backtrace(arry, sizeof(arry));

	for (size_t i = 1; i < size; i++)
	{
		if (arry[i]) printf("%p \n", arry[i]);
	}
}

inline void printlog(int fc, const char* buf, va_list args)
{
	fprintf(stdout,"\033[%dm", fc);
	vfprintf(stdout, buf, args);
	fprintf(stdout,"\033[0m");
}

#define print(lv, name, ver, fc, file, line, func, fmt)							\
do{																					\
	if (m_printLogLevel < (lv)) break;												\
	char buffer[8192];																\
	buffer[8191] = 0;																\
	int n = 0;																		\
	switch(m_logType)																							\
	{																											\
		case type_fileMsg:																						\
		n = snprintf(buffer, sizeof(buffer) - 1, "%s:%d %s ", (file), (line), (func));							\
		case type_modMsg:																						\
			break;																								\
		n = snprintf(buffer, sizeof(buffer) - 1, "[%s-%s] %s:%d %s ", (name), (ver), (file), (line), (func));	\
			break;																								\
		case type_onlyLog:																						\
		default:																								\
			break;																								\
	}																											\
	strncpy(buffer + n, (fmt), sizeof(buffer) - 1 - n);								\
	va_list args;																	\
	va_start(args, (fmt));															\
	printlog((fc), buffer, args);													\
	va_end(args);																	\
}while(0)																			\

void exprintf(int fc, int bc, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stdout,"\033[%d;%dm", fc, bc);
	vfprintf(stdout, fmt, args);
	fprintf(stdout,"\033[0m");
	va_end(args);

}

void exprintf(int fc, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stdout,"\033[%dm", fc);
	vfprintf(stdout, fmt, args);
	fprintf(stdout,"\033[0m");
	va_end(args);

}

CLog::CLog(int logType, std::string name, std::string ver)
:m_logType(logType)
,m_printLogLevel(logLevel_5)
,m_name(name)
,m_ver(ver)
{

}

CLog::~CLog()
{

}
int CLog::setLogLevel(int lv)
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

void CLog::_info(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_5, m_name.c_str(), m_ver.c_str(), Font_white, file, line, func, fmt);
}

void CLog::_debug(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_4, m_name.c_str(), m_ver.c_str(), Font_green, file, line, func, fmt);
}

void CLog::_trace(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_3, m_name.c_str(), m_ver.c_str(), Font_violet, file, line, func, fmt);
}

void CLog::_warning(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_2, m_name.c_str(), m_ver.c_str(), Font_yellow, file, line, func, fmt);
}

void CLog::_error(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_1, m_name.c_str(), m_ver.c_str(), Font_red, file, line, func, fmt);
}


CLog* CGlobalLog::instance()
{
	static CLog inst(CLog::type_fileMsg, std::string(""), std::string(""));
	return &inst;
}

CGlobalLog::CGlobalLog()
{

}

CGlobalLog::~CGlobalLog()
{

}

class CLogManager
{
private:
	CLogManager();
	~CLogManager()
public:
	static CLogManager* instance();
	
	CLog* getLog(std::string name);
private:
	static Infra::CMutex m_mutex;
	std::map<std::string, CLog*> m_mapLog;
};

CLogManager::CLogManager()
{

}
CLogManager::~CLogManager()
{
	std::map<std::string, CLog *>::iterator iter;
	for (iter = m_mapLog.begin(); iter != m_mapLog.end();)
	{
		delete iter->second;
		m_mapLog.erase(iter++);
	}
}

static CLogManager* CLogManager::instance()
{
	static CLogManager inst;
	return &inst;
}

bool CLogManager::getLog(std::string name)
{
	Infra::CGuard<Infra::CMutex> guard(m_mutex);
	std::map<std::string, CLog*>::iterator iter = m_mapLog.find(name);
	if (iter == m_mapLog.end())
	{
		CLog* p = new CTcpServer(port);
		m_mapLog.insert(std::pair<std::string, CLog*>(name, p));
		return p;
	}

	return iter->second;
}
