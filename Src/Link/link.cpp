#include "stdio.h"
#include "link.h"

namespace Infra
{

struct Node
{
	Node():prv(NULL),next(NULL),payload(NULL) {}
	struct Node* prv;
	struct Node* next;
	void* payload;
};

LinkManager::LinkManager()
:begin(NULL),end(NULL),iNode(0)
{

}

LinkManager::~LinkManager()
{

}

CLink::CLink()
:m_manager()
{

}

CLink::~CLink()
{
	releas();
}

int CLink::insert(void* payload, const unsigned int pos)
{
	if (m_manager.iNode == 0)
	{
		return rise(payload);
	}

	//插在当前pos前，使插入的节点成为pos，前向插入
	struct Node* temp = find(pos);
	if (temp == NULL)
	{
		return -1;
	}

	struct Node* inp = new struct Node;
	//装载负载
	inp->payload = payload;
	//左侧连接
	if (temp->prv != NULL)
	{
		(temp->prv)->next = inp;
		inp->prv = temp->prv;
	}
	else
	{
		//插入到头节点位置左侧不需要连接
		m_manager.begin = inp;
	}
	
	//右侧连接
	temp->prv = inp;
	inp->next = temp;

	return m_manager.iNode++;
}

int CLink::remove(void** payload, const unsigned int pos)
{
	if (m_manager.iNode == 0)
	{
		return -1;
	}
	struct Node* temp = find(pos);
	if (temp == NULL)
	{
		return -1;
	}
	//卸下负载
	*payload = temp->payload;
	//左侧断开
	if (temp->prv != NULL)
	{
		(temp->prv)->next = temp->next;
	}
	else
	{
		//要移除的是头节点
		m_manager.begin = temp->next;
	}
	
	//右侧断开
	if (temp->next != NULL)
	{
		(temp->next)->prv = temp->prv;
	}
	else
	{
		//要移除的是尾节点
		m_manager.end = temp->prv;
	}
	
	delete temp;
	
	return m_manager.iNode--;
}

int CLink::rise(void* payload)
{
	struct Node* p = new struct Node;
	p->payload = payload;
	p->prv = m_manager.end;
	if (m_manager.end != NULL)
	{
		m_manager.end->next = p;
	}
	m_manager.end = p;

	//添加的是第一个节点
	if (m_manager.iNode == 0 && m_manager.begin == NULL)
	{
		 m_manager.begin = m_manager.end;
	}
	return m_manager.iNode++;
}

int CLink::reduce(void** playload)
{
	if (m_manager.iNode == 0)
	{
		return -1;
	}

	struct Node* p = m_manager.end;
	m_manager.end = p->prv;
	if ( m_manager.end != NULL)
	{
		m_manager.end->next = NULL;
	}
	*playload = p->payload;
	delete p;
	return m_manager.iNode--;
}

void* CLink::get(const unsigned int pos)
{
	struct Node* p = find(pos);
	return p == NULL ? NULL : p->payload;
}

int CLink::linkSize() const
{
	return m_manager.iNode;
}

Node* CLink::find(const unsigned int pos)
{
	if (pos >= m_manager.iNode)
	{
		return NULL;
	}
	struct Node* p = m_manager.begin;
	for (size_t i = 0; i < pos; i++)
	{
		p = p->next;
	}
	return p;
}

void CLink::releas()
{
	struct Node* p;
	while (m_manager.iNode)
	{
		p = m_manager.end;
		m_manager.end = m_manager.end->prv;
		m_manager.end->next = NULL;
		delete p;
		m_manager.iNode--;
	}
	m_manager.end = NULL;
	m_manager.begin = NULL;
}

}//__LINK_H__
