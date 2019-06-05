#ifndef __LINK_H__
#define __LINK_H__

namespace Infra
{

struct Node;
struct LinkManager
{
	LinkManager();
	virtual ~LinkManager();
	struct Node* begin;
	struct Node* end;
	unsigned int iNode;
};

class CLink
{
public:
	CLink();
	virtual ~CLink();
	int insert(void* payload, const unsigned int pos);
	int remove(void** payload, const unsigned int pos);
	int rise(void* payload);
	int reduce(void** playload);
	void* get(const unsigned int pos);
	unsigned int linkSize() const;
	void clear();
private:
	Node* find(const unsigned int pos);
	void releas();
	LinkManager m_manager;
};

}//Infra

#endif //__LINK_H__
