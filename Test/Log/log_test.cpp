#include "Log.h"
#include "stdio.h"

using namespace Infra;
void Log_test(void)
{
	print_backtrace();

	Info("logTest","#1 %s %d\n", "&", 1);
	Debug("logTest","#2 %s %d\n", "&", 2);
	Trace("logTest", "#3 %s %d\n", "&", 3);
	Warning("logTest", "#4 %s %d\n", "&", 4);
	Error("logTest", "#5 %s %d\n", "&", 5);
	
	CLog* pLog = CLogManager::instance()->getLog("logTest");
	pLog->setLevel(CLog::logLevel_0);
	Info("logTest","#1 %s %d\n", "&", 1);
	Debug("logTest","#2 %s %d\n", "&", 2);
	Trace("logTest", "#3 %s %d\n", "&", 3);
	Warning("logTest", "#4 %s %d\n", "&", 4);
	Error("logTest", "#5 %s %d\n", "&", 5);
	
	CLog inst(std::string(""), std::string(""), CLog::type_fileMsg);
	inst.setColor(true);
	inst.info("#1 CLog %s %d\n", "&", 1);
	inst.debug("#2 CLog %s %d\n", "&", 2);
	inst.trace("#3 CLog %s %d\n", "&", 3);
	inst.warning("#4 CLog %s %d\n", "&", 4);
	inst.error("#5 CLog %s %d\n", "&", 5);

	CLogManager::instance()->setLevel(CLog::logLevel_5);
	CLogManager::instance()->setType(CLog::type_fileMsg);
	CLogManager::instance()->setColor(true);

	CLogManager::instance()->setoutput("./log.txt");
	Info("logTest","/#1 %s %d\n", "&", 1);
	Debug("logTest","/#2 %s %d\n", "&", 2);
	Trace("logTest", "/#3 %s %d\n", "&", 3);
	Warning("logTest", "/#4 %s %d\n", "&", 4);
	Error("logTest", "/#5 %s %d\n", "&", 5);
	//printf("\033[40;35m""aaaaaaaaaaaaaaaaa""\033[0m\n");

}