
class CPacket
{
public:
	CPacket();
	virtual ~CPacket();
public:
	int append(char* pbuf, int len);
	int size() const;
	int capacity() const;
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

	struct Node* nodeRise(void);
	int totalUse;
	int nodeNum;

	struct Node* m_phead;
	struct Node* m_pTail;
	

};