#ifndef __LINK_H__
#define __LINK_H__

namespace Infra
{

struct LinkInternal;
class CLink
{
public:
	CTimer();
	virtual ~CTimer();
private:
	LinkInternal* m_internal
};

}//Infra
#endif //__LINK_H__
