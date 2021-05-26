#include "Log.h"
#include "stdio.h"
void Log_test(void)
{
	print_backtrace();

	Info("logTest","#1 %s %d\n", "&", 1);
	Debug("logTest","#2 %s %d\n", "&", 2);
	Trace("logTest", "#3 %s %d\n", "&", 3);
	Warning("logTest", "#4 %s %d\n", "&", 4);
	Error("logTest", "#5 %s %d\n", "&", 5);
	
	CLog* pLog = CLogManager::instance()->getLog("logTest");
	pLog->setLogLevel(CLog::logLevel_0);
	Info("logTest","#1 %s %d\n", "&", 1);
	Debug("logTest","#2 %s %d\n", "&", 2);
	Trace("logTest", "#3 %s %d\n", "&", 3);
	Warning("logTest", "#4 %s %d\n", "&", 4);
	Error("logTest", "#5 %s %d\n", "&", 5);
	
	CLog inst(CLog::type_fileMsg, std::string(""), std::string(""));
	inst.info("#1 CLog %s %d\n", "&", 1);
	inst.debug("#2 CLog %s %d\n", "&", 2);
	inst.trace("#3 CLog %s %d\n", "&", 3);
	inst.warning("#4 CLog %s %d\n", "&", 4);
	inst.error("#5 CLog %s %d\n", "&", 5);


	//printf("\033[40;35m""aaaaaaaaaaaaaaaaa""\033[0m\n");

}