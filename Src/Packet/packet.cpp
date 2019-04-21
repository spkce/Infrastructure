
#include <stdio.h>
#include <string.h>
#include "packet.h"
 


CPacket::CPacket()
:totalUse(0)
,nodeNum(0)
,m_phead(NULL)
,m_pTail(NULL)
{

}

CPacket::~CPacket()
{

}


int CPacket::append(char* pbuf, int len)
{
	char* p = pbuf;
	if (p == NULL || len == 0)
	{
		return 0;
	}

	int tailSurplus = 0;

	if (m_pTail != NULL) //非第一次添加
	{
		tailSurplus = Node::capacity_size - m_pTail->use;
		if (len > tailSurplus)
		{
			memcpy(m_pTail->cap + m_pTail->use, p, tailSurplus);
			p += tailSurplus;
			m_pTail->use += tailSurplus;
			len -= tailSurplus;
		}
		else
		{
			memcpy(m_pTail->cap + m_pTail->use, p, len);
			p += len;
			m_pTail->use += len;
			return len;
		}
	}
	
	for(int i = 0; len > Node::capacity_size*i; i++)
	{
		nodeRise();
		int cpyLen = (len - Node::capacity_size*i) > Node::capacity_size ? Node::capacity_size : (len - Node::capacity_size*i);
		memcpy(m_pTail->cap, p, cpyLen);
		p += cpyLen;
		m_pTail->use += cpyLen;
	}

	return len + tailSurplus;
}

struct CPacket::Node* CPacket::nodeRise(void)
{
	struct Node* temp = m_pTail;
	m_pTail = new struct Node;
	if(temp == NULL)
	{
		m_phead = m_pTail;
	}
	else
	{
		temp->next = m_pTail;
	}

	m_pTail->next = NULL;
	m_pTail->use = 0;
	nodeNum++;
	return temp;
	
}