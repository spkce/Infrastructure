#include "stdio.h"
#include "callback.h"

#include "TFuncation.h"

class CTest
{
public:
	CTest():m_sig()
	{
		m_test = 0;
		m_sig.attach(this, &CTest::onCallback);
	}
	~CTest(){}
public: 
	void onCallback(int a, int b)
	{
		printf("CTest::onCallback(%d,%d)\n",a,b);
		m_test = a;
		printf("m_test = %d \n", m_test);
	}
	void dump()
	{
		printf("CTest::dump()\n");
		m_sig(1,1);
	}
	Infra::CCallback<CTest, int, int> m_sig;
	int m_test;
};

class CTest2
{
public:
	CTest2():m_sig(23)
	{
		m_test = 0;
		m_sig.attach(this, &CTest2::onCallback0);
		m_sig.attach(this, &CTest2::onCallback1);
		m_sig.attach(this, &CTest2::onCallback2);
	}
	~CTest2(){}
public: 
	void onCallback0(int a, int b)
	{
		printf("CTest2::onCallback0(%d,%d)\n",a,b);
		m_test = a;
		printf("m_test = %d \n", m_test);
	}
	void onCallback1(int a, int b)
	{
		printf("CTest2::onCallback1(%d,%d)\n",a,b);
		m_test = a;
		printf("m_test = %d \n", m_test);
	}
	void onCallback2(int a, int b)
	{
		printf("CTest2::onCallback2(%d,%d)\n",a,b);
		m_test = a;
		printf("m_test = %d \n", m_test);
	}
	void dump()
	{
		printf("CTest2::dump()\n");
		m_sig(1,1);
	}
	Infra::CObserver<CTest2, int, int> m_sig;
	int m_test;
};

class CTFunctionTest
{
public:
	void fun(int a, int b);
	void dump()
	{
		//TFuncation1<void, int, int> ss(&CTFunctionTest::fun, this);

		//ss
	}
};

void callback_test(void)
{
	{
		CTest test;
		test.dump();
	}
	{
		CTest2 test;
		test.dump();
	}
	{
		CTFunctionTest test;

		test.dump();
	}
}