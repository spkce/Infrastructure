
#include "link.h"

namespace Infra
{
template<class T, typename P1, typename P2>
class CCallBack
{
public:
	CCallBack(unsigned int maxAttach);
	virtual ~CCallBack();
	typedef void (T::*cbfun)(P1, P2);
	bool attach(T* pInst, cbfun fun);
	bool detach(T* pInst, cbfun fun);
private:
	struct AttachInfo
	{
		T* instance;
		cbfun fun;
	};

	unsigned int m_maxAttach;
	Clink m_link;
};

CCallBack::CCallBack(unsigned int maxAttach)
:m_maxAttach(maxAttach)
,m_link()
{

}

CCallBack::~CCallBack()
{
	struct AttachInfo * pInfo;
	while(m_link.reduce((void*)&pInfo) > 0)
	{
		delete pInfo;
	}
}

bool CCallBack::attach(T* pInst, cbfun fun)
{
	struct AttachInfo * pInfo = new struct AttachInfo;
	pInfo->instance = pInst;
	pInfo->fun = fun;
	m_link.rise((void*)pInfo);
}

bool CCallBack::detach(T* pInst, cbfun fun)
{

}


}
