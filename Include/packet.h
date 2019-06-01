
#ifndef __PACKET_H__
#define __PACKET_H__

//class CSlider
class CPacket
{
public:
	CPacket();
	virtual ~CPacket();
public:
	int append(char* pbuf, int len);
	int size() const;
	int capacity() const;
	char operator[](const int n);
private:
	struct Node
	{
		enum
		{
			capacity_size = 4*1024
		};
		char cap[capacity_size];
		int use;
		struct Node* next;
	};
	//增加一个尾节点
	struct Node* nodeRise(void);
	struct Node* getNodePos(const int n);
	int m_totalUse;
	int m_nodeNum;

	struct Node* m_phead;
	struct Node* m_pTail;
	

};

#endif //__PACKET_H__