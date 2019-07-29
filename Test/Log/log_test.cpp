#include "Log.h"

void Log_test(void)
{
	print_backtrace();

	Info("#1 %s %d\n", "&", 1);
	Debug("#2 %s %d\n", "&", 2);
	Trace("#3 %s %d\n", "&", 3);
	Warning("#4 %s %d\n", "&", 4);
	Error("#5 %s %d\n", "&", 5);

	CLog inst(CLog::type_fileMsg, std::string(""), std::string(""));
	inst.info("#1 CLog %s %d\n", "&", 1);
	inst.debug("#2 CLog %s %d\n", "&", 2);
	inst.trace("#3 CLog %s %d\n", "&", 3);
	inst.warning("#4 CLog %s %d\n", "&", 4);
	inst.error("#5 CLog %s %d\n", "&", 5);
}