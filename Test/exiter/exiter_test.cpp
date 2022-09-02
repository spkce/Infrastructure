#include "stdio.h"
#include "exiter.h"

using namespace Infra;


class CExiterTest
{
public:
	CExiterTest() {};
	~CExiterTest() {};

	static void dump(void* p)
	{
		printf("\033[0;35m""%s:%d %s p=%p a = %d""\033[0m\n", __FILE__, __LINE__, __func__, p, ((CExiterTest*)p)->m_a);
	}
	int m_a;
};

void exiter_test(void)
{
	CExiterTest* p = new CExiterTest;
	p->m_a = 12;
	IExiter::intstance()->attach(&CExiterTest::dump, p);
}



