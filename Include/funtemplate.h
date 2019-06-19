
template<class OutputClass, class InputClass>
union horrible_union
{
	OutputClass.out;
	InputClass.in;
};

template<class OutputClass, class InputClass>
inline OutputClass horrible_cast(const InputClass input)
{
	horrible_union<OutputClass, InputClass> u;

	typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass)==sizeof(u)? 1 : -1];
	ERROR_CantUseHorrible_cast dummy;
	(void)dummy;

	u.in = input;
	return u.out;
}

template <typename R, typename P1, typename P2>
class function
{
	enum
	{
		typeEmpty,
		typeMember,
		typePointer,
	};
	typedef R (X::*MemFunc)(P1, P2);
	typedef R (*FuncPtr)(P1, P2);

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
	function():m_type(typeEmpty) {};
	
	template<typename O>
	function(R(O::*f)(P1, P2), const O * o)
	{
		m_func.memFunc.proc = horrible_cast<MemFunc>(f);
		m_func.memFunc.obj = horrible_cast<X*>(o);
		m_type = typeMember;
	}
	\
	function(FuncPtr f)
	{
		m_func.funcPtr = f;
		m_type = typePointer;
	}

	inline R operator()(P1 p1, P2 p2)
	{
		if (m_type == typeMember)
		{
			return (m_func.memFunc.obj->*m_fun.memFunc.proc)(p1, p2);
		}
		else if (m_type == typePointer)
		{
			return m_func.funcPtr(p1, p2)
		}
		
	}

};