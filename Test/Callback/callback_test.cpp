#include "stdio.h"
#include "callback.h"
#include "cfunc.h"

void callback4(void* p)
{
    printf("callback4 a:%p\n", p);
}
class CTest
{
    Infra::CFunc<void> m_sig0;
    Infra::CFunc<int, int, char> m_sig1;
    Infra::CFunc<int, int&&, int&> m_sig2;
    Infra::CFunc<void, int, int> m_sig3;
    Infra::CFunc<void, void*> m_sig4;
    void callback0()
    {
        printf("callback0\n");
    }
    int callback1(int a, char b)
    {
        printf("callback1 a:%d b:%d\n", a, (int)b);
        return a+(int)b;
    }
    int callback2(int && a, int &b)
    {
        printf("callback2 a:%d b:%d\n", a, --b);
        return a+b;
    }
    static void callback3(int a, int b)
    {
        printf("callback3 a:%d b:%d\n", a, b);
    }
    

public:
    CTest(/* args */)
    {
        m_sig0.bind(&CTest::callback0, this);
        m_sig1.bind(&CTest::callback1, this);
        m_sig2.bind(&CTest::callback2, this);
        m_sig3.bind(&CTest::callback3);
        m_sig4.bind(callback4);
    }
    
    ~CTest()
    {
        m_sig0.unbind();
        m_sig1.unbind();
        m_sig2.unbind();
        m_sig3.unbind();
    }

    void dump()
    {
        m_sig0();
       int ret1 = m_sig1(10,'1');
       printf("m_sig1 ret:%d\n", ret1);
       
       int a = 10;
       int b = 11;
       int ret2 = m_sig2(std::move(a), b);
       printf("m_sig2 ret:%d b:%d\n", ret2, b);
       m_sig3(10,11);

       void* p = &a;
       m_sig4(p);
    }
};


void callback_test(void)
{
	printf("**************callback_test***************\n");
	CTest test;
	test.dump();
}