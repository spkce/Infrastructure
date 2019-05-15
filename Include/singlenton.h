#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "thread.h"

#undef PATTERN_SINGLETON_DECLARE
#define PATTERN_SINGLETON_DECLARE(classname)	\
public:											\
	static classname* instance()				\
private:										\
	classname()									\
	~classname()								\
	friend class std::auto_ptr<classname>		\


#undef PATTERM_SINGLETON_IMPLEMENT
#define PATTERM_SINGLETON_IMPLEMENT(classname)	\
static std::auto_ptr<classname>					\
void exit##classname(void)						\
{												\
	instance##classname = std::auto_ptr<classname>(NULL)	\
}												\
classname * class::instance()						\
{												\
	if (NULL == instance##classname.get())		\
	{											\
		static Infra::CMutex sm_mutex;			\
		CGuard<Infra::CMutex> guard(sm_mutex##classname);		\
		if (NULL == instance##classname.get())	\
		{										\
			instanc##classname = std::auto_ptr<classname>(new classname)	\
			if(0 != atexit(exit##classname))	\
			{									\
				printf("%s:%s:%s atexit faild\n",__FILE__,__FUNCTION__,__LINE__)	\
			}									\
		}										\
	}											\
	return instance##classname.get();			\
}												\




#endif //__SINGLETON_H__
