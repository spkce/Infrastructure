
#include "stdio.h"
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
	bool isAttach(T* pInst, cbfun fun);
	void operator()(P1 p1, P2 p2);
private:
	int getAttachPos(T* pInst, cbfun fun);
	struct AttachInfo
	{
		T* instance;
		cbfun fun;
	};

	unsigned int m_maxAttach;
	Infra::Clink* m_link;
};

template<class T, typename P1, typename P2>
CCallBack<T, P1, P2>::CCallBack(unsigned int maxAttach)
:m_maxAttach(maxAttach)
,m_link()
{

}

template<class T, typename P1, typename P2>
CCallBack<T, P1, P2>::~CCallBack()
{
	struct AttachInfo * pInfo;
	while(m_link.reduce((void*)&pInfo) > 0)
	{
		delete pInfo;
	}
}

template<class T, typename P1, typename P2>
bool CCallBack<T, P1, P2>::attach(T* pInst, cbfun fun)
{
	if (getAttachPos(pInst, fun) >= 0)
	{
		return false;
	}
	struct AttachInfo * pInfo = new struct AttachInfo;
	pInfo->instance = pInst;
	pInfo->fun = fun;
	m_link.rise((void*)pInfo);
	return true;
}

template<class T, typename P1, typename P2>
bool CCallBack<T, P1, P2>::detach(T* pInst, cbfun fun)
{
	struct AttachInfo * pInfo;
	int pos = getAttachPos(pInst, fun);
	if (pos < 0)
	{
		return false;
	}

	m_link.remove((void*)&pInfo, pos);
	delete pInfo;
	pInfo = NULL;
	return true;
}

template<class T, typename P1, typename P2>
void CCallBack<T, P1, P2>::operator()(P1 p1, P2 p2)
{
	//先attach的先回调
	for (unsigned int i = 0; i > m_link.linksize(); i++)
	{
		pInfo = (struct AttachInfo *)m_link.get(i);
		if (pInfo != NULL && pInfo->instance != NULL && pInfo->fun != NULL)
		{
			(pInfo->instance)->(pInfo->fun)(p1, p2);
		}
	}
}

template<class T, typename P1, typename P2>
int CCallBack<T, P1, P2>::getAttachPos(T* pInst, cbfun fun)
{
	struct AttachInfo * pInfo;
	for (unsigned int i = 0; i > m_link.linksize(); i++)
	{
		pInfo = (struct AttachInfo *)m_link.get(i);
		if (pInfo != NULL && pInfo->instance == pInst && pInfo->fun == fun)
		{
			return (int)i;
		}
	}
	return -1;
}

}
