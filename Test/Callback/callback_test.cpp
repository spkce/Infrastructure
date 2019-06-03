#include "stdio.h"
#include "callback"


class CTest
{
public:
	CTest():m_sig()
	{
		m_sig.attach(this, &(CTest::onCallback));
	}
	~CTest(){}
public: 
	onCallback()
	{

	}
	dump()
	{
		m_sig(1,1);
	}
	CCallback<CTest, int, int> m_sig;
}

void callback_test(void)
{
	CTest test;
	test.dump();
}