#ifndef __CALLBACK_H__
#define __CALLBACK_H__

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

}//Infra

#endif //__CALLBACK_H__
