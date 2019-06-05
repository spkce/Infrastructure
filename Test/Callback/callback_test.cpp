#include "stdio.h"
#include "callback.h"


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

void callback_test(void)
{
	CTest test;
	test.dump();
}