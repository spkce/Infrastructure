
//该文件需要多次引用，此处不能使用宏防止多次引用

#include "def.h"

#define CLASS_NAME TFuncation

#define FUCTION_TEMPLATE INFRA_JOIN(CLASS_NAME, PARAM_NUM)

template <typename R, TEMPLATE_PARAM_TYPE>
class FUCTION_TEMPLATE
{
	class X {};
	enum
	{
		typeEmpty,
		typeMember,
		typePointer,
	};
	typedef R (X::*MemFunc)(PARAM_LIST);
	typedef R (*FuncPtr)(PARAM_LIST);

	union 
	{
		struct 
		{
			MemFunc proc;
			X* obj;
		}memFunc;
		FuncPtr funcPtr;
	}m_func;

	int m_type;
public:
	FUCTION_TEMPLATE():m_type(typeEmpty) {};
	
	template<typename O>
	FUCTION_TEMPLATE(R(O::*f)(PARAM_LIST), const O * o)
	{
		m_func.memFunc.proc = horrible_cast<MemFunc>(f);
		m_func.memFunc.obj = horrible_cast<X*>(o);
		m_type = typeMember;
	}
	
	FUCTION_TEMPLATE(FuncPtr f)
	{
		m_func.funcPtr = f;
		m_type = typePointer;
	}

	inline R operator()(PARAM_LIST)
	{
		if (m_type == typeMember)
		{
			return (m_func.memFunc.obj->*m_func.memFunc.proc)(PARAM_INPUT);
		}
		else if (m_type == typePointer)
		{
			return m_func.funcPtr(PARAM_INPUT);
		}
	}
};
