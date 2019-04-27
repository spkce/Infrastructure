
#include <stdio.h>
#include <string.h>
#include "packet.h"
 


CPacket::CPacket()
:m_totalUse(0)
,m_nodeNum(0)
,m_phead(NULL)
,m_pTail(NULL)
{

}

CPacket::~CPacket()
{

}

int CPacket::size() const
{
	return m_totalUse;
}

int CPacket::capacity() const
{
	return m_nodeNum * Node::capacity_size;
}

int CPacket::append(char* pbuf, int len)
{
	char* p = pbuf;
	if (p == NULL || len == 0)
	{
		return 0;
	}

	m_totalUse += len;
	int tailSurplus = 0;

	if (m_pTail != NULL) //非第一次添加
	{
		//计算当前尾部节点剩余容量
		tailSurplus = Node::capacity_size - m_pTail->use;
		if (len > tailSurplus)
		{
			//尾部节点剩余容量不够
			memcpy(m_pTail->cap + m_pTail->use, p, tailSurplus);
			p += tailSurplus;
			m_pTail->use += tailSurplus;
			len -= tailSurplus;
		}
		else
		{
			//尾部节点剩余容量足够
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
	m_nodeNum++;
	return temp;
	
}

struct CPacket::Node* CPacket::getNodePos(int n)
{
	//n从0开始计数， m_nodeNum从1开始计数
	if (n >= m_nodeNum || n < 0)
	{
		return NULL;
	}

	CPacket::Node* temp = m_phead;

	while(n > 0)
	{
		temp = temp->next;
		n--;
	}
	
	return temp;
}

char CPacket::operator[](const int n)
{
	int nodePos = n / Node::capacity_size;
	int nPos = n % Node::capacity_size;

	CPacket::Node* temp = getNodePos(nodePos);
	
	return temp->cap[nPos];

	
}
