#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "execinfo.h"
//#include <fstream> 
#include "Log.h"
#include <stdarg.h>

namespace Infra
{
class CLogOut
{
	CLogOut() {}
	virtual ~CLogOut() {}
public:
	static CLogOut* instance()
	{
		static CLogOut inst;
		return &inst;
	}

	bool isInit()
	{
		bool ret = false;
		m_rwlock.rLock();
		ret = !m_file.empty();
		m_rwlock.unLock();
		return ret;
	}
	
	bool init(const std::string & file)
	{
		if (!isInit())
		{
			m_rwlock.wLock();
			if (m_file.empty())
			{
				m_file = file;
				m_rwlock.unLock();
				return true;
			}
			m_rwlock.unLock();
			return true;
		}

		return false;
	}

	void stop()
	{
		m_rwlock.wLock();
		m_file.clear();
		m_rwlock.unLock();
	}

	void out(const char* buf, va_list args)
	{
		FILE* fp;
		m_rwlock.rLock();
		fp = fopen(m_file.c_str(), "a");
		m_rwlock.unLock();
		if (fp) 
		{
			vfprintf(fp, buf, args);
			fflush(fp);
			fclose(fp);
		}
	}
private:
	Infra::CRwlock m_rwlock;
	std::string m_file;
};

void print_backtrace()
{
	void * arry[10] = {0};

	size_t size = backtrace(arry, sizeof(arry));

	for (size_t i = 1; i < size; i++)
	{
		if (arry[i]) printf("%p \n", arry[i]);
	}
}

inline void writelog(const char* buf, va_list args)
{
	if (CLogOut::instance()->isInit())
	{
		CLogOut::instance()->out(buf, args);
	}
}

inline void printlog(int fc, const char* buf, va_list args)
{
	if(fc) fprintf(stdout,"\033[%dm", fc);
	vfprintf(stdout, buf, args);
	if(fc) fprintf(stdout,"\033[0m");
}

#define print(lv, name, ver, fc, file, line, func, fmt)							\
do{																					\
	if (m_level < (lv)) break;												\
	char buffer[8192];																\
	buffer[8191] = 0;																\
	int n = 0;																		\
	switch(m_type)																							\
	{																											\
		case type_fileMsg:																						\
			n = snprintf(buffer, sizeof(buffer) - 1, "%s:%d %s ", filename(file), (line), (func));						\
			break;																								\
		case type_modMsg:																						\
			n = snprintf(buffer, sizeof(buffer) - 1, "[%s] %s:%d %s ", (name), filename(file), (line), (func));			\
			break;																								\
		case type_modVer:																						\
			n = snprintf(buffer, sizeof(buffer) - 1, "[%s-%s] %s:%d %s ", (name), filename(ver), (file), (line), (func));\
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
	va_list argvs;																	\
	va_start(argvs, (fmt));															\
	writelog(buffer, argvs);		\
	va_end(argvs);																	\
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

CLog::CLog(const std::string & name, const std::string & ver, int type)
:m_type(type)
,m_level(logLevel_0)
,m_name(name)
,m_ver(ver)
,m_isColorOn(false)
{

}

CLog::~CLog()
{

}

int CLog::setLevel(int lv)
{
	int ret = m_level;
	m_level = lv;
	return ret;
}

int CLog::setType(int type)
{
	int ret = m_type;
	m_type = type;
	return ret;
}

void CLog::setColor(bool isOn)
{
	m_isColorOn = isOn;
}

int CLog::getLevel() const
{
	return m_level;
}

int CLog::getType() const
{
	return m_type;
}

bool CLog::isColorOn() const
{
	return m_isColorOn;
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
	print(logLevel_5, m_name.c_str(), m_ver.c_str(), m_isColorOn ? Font_white : 0, file, line, func, fmt);
}

void CLog::_debug(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_4, m_name.c_str(), m_ver.c_str(), m_isColorOn ? Font_green : 0, file, line, func, fmt);
}

void CLog::_trace(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_3, m_name.c_str(), m_ver.c_str(), m_isColorOn ? Font_violet : 0, file, line, func, fmt);
}

void CLog::_warning(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_2, m_name.c_str(), m_ver.c_str(), m_isColorOn ? Font_yellow : 0, file, line, func, fmt);
}

void CLog::_error(const char* file, int line, const char* func, const char* fmt, ...)
{
	print(logLevel_1, m_name.c_str(), m_ver.c_str(), m_isColorOn ? Font_red : 0, file, line, func, fmt);
}


CLogManager::CLogManager()
:m_type(CLog::type_onlyLog)
,m_level(CLog::logLevel_0)
,m_isColorOn(false)
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

CLogManager* CLogManager::instance()
{
	static CLogManager inst;
	return &inst;
}

CLog* CLogManager::getLog(const std::string & name)
{
	//Infra::CGuard<Infra::CMutex> guard(m_mutex);
	std::map<std::string, CLog*>::iterator iter;
	
	m_rwlock.rLock();
	iter = m_mapLog.find(name);
	m_rwlock.unLock();

	if (iter == m_mapLog.end())
	{
		m_rwlock.wLock();
		CLog* p = new CLog(name, "", m_type);
		p->setLevel(m_level);
		p->setColor(m_isColorOn);
		m_mapLog.insert(std::pair<std::string, CLog*>(name, p));
		m_rwlock.unLock();
		return p;
	}

	return iter->second;
}

CLog* CLogManager::findLog(const std::string & name)
{
	std::map<std::string, CLog*>::iterator iter;
	
	m_rwlock.rLock();
	iter = m_mapLog.find(name);
	m_rwlock.unLock();
	
	if (iter == m_mapLog.end())
	{
		return NULL;
	}
	
	return iter->second;
}

bool CLogManager::setoutput(const std::string & file)
{
	if (!file.empty())
	{
		if ( !CLogOut::instance()->isInit())
		{
			return CLogOut::instance()->init(file);
		}
	}
	else
	{
		if ( CLogOut::instance()->isInit())
		{
			CLogOut::instance()->stop();
			return false;
		}
	}
	return false;
}

void CLogManager::setLevel(int lv)
{
	m_level = lv;
	m_rwlock.rLock();
	for (std::map<std::string, CLog*>::iterator it = m_mapLog.begin(); it != m_mapLog.end(); it++)
	{
		CLog* p = it->second;
		p->setLevel(lv);
	}
	m_rwlock.unLock();
}

void CLogManager::setType(int type)
{
	m_type = type;
	m_rwlock.rLock();
	for (std::map<std::string, CLog*>::iterator it = m_mapLog.begin(); it != m_mapLog.end(); it++)
	{
		CLog* p = it->second;
		p->setType(type);
	}
	m_rwlock.unLock();
}

void CLogManager::setColor(bool isOn)
{
	m_isColorOn = isOn;
	m_rwlock.rLock();
	for (std::map<std::string, CLog*>::iterator it = m_mapLog.begin(); it != m_mapLog.end(); it++)
	{
		CLog* p = it->second;
		p->setColor(isOn);
	}
	m_rwlock.unLock();
}
//void ex_info(const char* file, int line, const char* func, const char* fmt, ...)
}//Infra
