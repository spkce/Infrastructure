
#include "stdio.h"
#include "link.h"

namespace Infra
{

template<class T, typename P1, typename P2>
class CCallback
{
public:
	CCallback();
	virtual ~CCallback();
	typedef void (T::*cbfun)(P1, P2);
	bool attach(T* pInst, cbfun fun);
	bool detach(T* pInst, cbfun fun);
	bool isAttach();
	void operator()(P1 p1, P2 p2);
private:
	struct AttachInfo
	{
		T* instance;
		cbfun fun;
	};

	struct AttachInfo m_attchInfo;
};

template<class T, typename P1, typename P2>
CCallback<T, P1, P2>::CCallback()
{
	m_attchInfo.instance = NULL;
	m_attchInfo.fun = NULL;
}

template<class T, typename P1, typename P2>
CCallback<T, P1, P2>::~CCallback()
{

}

template<class T, typename P1, typename P2>
bool CCallback<T, P1, P2>::attach(T* pInst, cbfun fun)
{
	if (m_attchInfo.instance == NULL || m_attchInfo.fun == NULL)
	{
		return false;
	}
	
	m_attchInfo.instance = pInst;
	m_attchInfo.fun = fun;
	return true;
}

template<class T, typename P1, typename P2>
bool CCallback<T, P1, P2>::detach(T* pInst, cbfun fun)
{
	if (m_attchInfo.instance != pInst || m_attchInfo.fun != fun)
	{
		return false;
	}

	m_attchInfo.instance = NULL;
	m_attchInfo.fun = NULL;
}

template<class T, typename P1, typename P2>
bool CCallback<T, P1, P2>::isAttach()
{

	if (m_attchInfo.instance == NULL || m_attchInfo.fun == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

template<class T, typename P1, typename P2>
void CCallback<T, P1, P2>::operator()(P1 p1, P2 p2)
{
	if (m_attchInfo.instance != NULL && m_attchInfo.fun != NULL)
	{
		((m_attchInfo.instance)->*(m_attchInfo.fun))(p1, p2);
	}
}

template<class T, typename P1, typename P2>
class CObserver
{
public:
	CObserver(unsigned int maxAttach);
	virtual ~CObserver();
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
	CLink m_link;
};

template<class T, typename P1, typename P2>
CObserver<T, P1, P2>::CObserver(unsigned int maxAttach)
:m_maxAttach(maxAttach)
,m_link(NULL)
{

}

template<class T, typename P1, typename P2>
CObserver<T, P1, P2>::~CObserver()
{
	struct AttachInfo * pInfo;
	while(m_link.reduce((void**)&pInfo) > 0)
	{
		delete pInfo;
		pInfo = NULL;
	}
}

template<class T, typename P1, typename P2>
bool CObserver<T, P1, P2>::attach(T* pInst, cbfun fun)
{
	if (m_link.linkSize() >= m_maxAttach)
	{
		return false;
	}
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
bool CObserver<T, P1, P2>::detach(T* pInst, cbfun fun)
{
	struct AttachInfo * pInfo;
	int pos = getAttachPos(pInst, fun);
	if (pos < 0)
	{
		return false;
	}

	m_link.remove((void**)&pInfo, pos);
	delete pInfo;
	pInfo = NULL;
	return true;
}

template<class T, typename P1, typename P2>
bool CObserver<T, P1, P2>::isAttach(T* pInst, cbfun fun)
{
	if (getAttachPos(pInst, fun) < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

template<class T, typename P1, typename P2>
void CObserver<T, P1, P2>::operator()(P1 p1, P2 p2)
{
	struct AttachInfo * pInfo;
	//先attach的先回调
	for (unsigned int i = 0; i > m_link.linkSize(); i++)
	{
		pInfo = (struct AttachInfo *)m_link.get(i);
		if (pInfo != NULL && pInfo->instance != NULL && pInfo->fun != NULL)
		{
			((pInfo->instance)->*(pInfo->fun))(p1, p2);
		}
	}
}

template<class T, typename P1, typename P2>
int CObserver<T, P1, P2>::getAttachPos(T* pInst, cbfun fun)
{
	struct AttachInfo * pInfo;
	for (unsigned int i = 0; i > m_link.linkSize(); i++)
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
