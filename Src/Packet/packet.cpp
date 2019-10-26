
#include <stdio.h>
#include <string.h>
#include "packet.h"
 
CPacket::Node::Node()
{
	next = NULL;
	use = 0;
	memset(cap, 0, capacity);
}

CPacket::CPacket()
:m_iTotalLen(0)
,m_iNode(0)
,m_pHead(NULL)
,m_pTail(NULL)
,m_pCurNode(NULL)
,m_pBuffer(NULL)
{

}

CPacket::CPacket(unsigned int size)
:m_iTotalLen(0)
,m_iNode(0)
,m_pHead(NULL)
,m_pTail(NULL)
,m_pCurNode(NULL)
,m_pBuffer(NULL)
{
	if(size > 0)
	{
		m_pHead = riseNode();
		m_pCurNode = m_pHead;
		for (unsigned int i = 0; i < size / Node::capacity; i++)
		{
			riseNode();
		}
	}
}

CPacket::~CPacket()
{
	Node_t* p = NULL;
	while (m_pHead != NULL)
	{
		p = m_pHead->next;
		delete m_pHead;
		m_pHead = p;
	}

	if (m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
	}
	m_pBuffer = NULL;
	m_pTail = NULL;
	m_pCurNode = NULL;
	m_iTotalLen = 0;
	m_iNode = 0;
}

int CPacket::size() const
{
	return m_iTotalLen;
}

int CPacket::capacity() const
{
	return m_iNode * Node::capacity;
}

bool CPacket::append(const char* pbuf, int len)
{
	char* p = (char*)pbuf;
	if (p == NULL || len <= 0)
	{
		return false;
	}

	if (m_pHead == NULL)
	{
		m_pHead = riseNode();
		m_pCurNode = m_pHead;
	}
	
	int iCopyLen = m_pCurNode->capacity - m_pCurNode->use;

	if (iCopyLen >= len)
	{
		memcpy(m_pCurNode->cap + m_pCurNode->use, p, len);
		m_iTotalLen += len;
	}
	else
	{
		memcpy(m_pCurNode->cap + m_pCurNode->use, p, iCopyLen);
		m_pCurNode->use = m_pCurNode->capacity;
		p += iCopyLen;
		m_iTotalLen += iCopyLen;

		iCopyLen = 0;
		while (len > 0)
		{
			p += iCopyLen;

			if (m_pCurNode->next == NULL)
			{
				m_pCurNode = riseNode();
			}
			else
			{
				m_pCurNode = m_pCurNode->next;
			}

			iCopyLen = len > m_pCurNode->capacity ? m_pCurNode->capacity : len;
			memcpy(m_pCurNode->cap, p, iCopyLen);
			len -= iCopyLen;
			m_iTotalLen += iCopyLen;
		}
	}
	return true;
}

void CPacket::clear()
{
	Node_t* p = NULL;
	while (m_pHead != NULL)
	{
		p = m_pHead->next;
		delete m_pHead;
		m_pHead = p;
	}

	if (m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
	}
	m_pBuffer = NULL;
	m_pTail = NULL;
	m_pCurNode = NULL;
	m_iTotalLen = 0;
	m_iNode = 0;
}

char* CPacket::getBuffer()
{
	static int totalLen = 0;
	if (m_pHead == NULL || m_iTotalLen <= 0)
	{
		return NULL;
	}

	if (totalLen == m_iTotalLen && m_pBuffer != NULL)
	{
		return m_pBuffer;
	}

	if (m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
	}

	m_pBuffer = new char[m_iTotalLen + 1];
	if (m_pBuffer == NULL)
	{
		return NULL;
	}

	m_pBuffer[m_iTotalLen] = 0;

	Node_t* p = m_pHead;
	char* pBuf = m_pBuffer;
	do
	{
		memcpy(pBuf, p->cap, p->use);
		pBuf += p->use;
		if (p->use != Node::capacity)
		{
			break;
		}
		p = p->next;
	} while(p != NULL);

	return m_pBuffer;
}

CPacket::Node_t* CPacket::riseNode(void)
{
	Node_t* p = new Node_t;
	if(p == NULL)
	{
		return NULL;
	}
	
	if (m_pTail == NULL)
	{
		m_pTail->next = p;
	}

	m_pTail = p;
	m_iNode++;
	return p;
	
}

CPacket::Node_t* CPacket::getNodePos(int n)
{
	//n从0开始计数， m_iNode从1开始计数
	if (n >= m_iNode || n < 0)
	{
		return NULL;
	}

	CPacket::Node* temp = m_pHead;

	while(n > 0)
	{
		temp = temp->next;
		n--;
	}
	
	return temp;
}

char CPacket::operator[](const int n)
{
	int nodePos = n / Node::capacity;
	int nPos = n % Node::capacity;

	CPacket::Node* temp = getNodePos(nodePos);
	
	return temp->cap[nPos];
}
