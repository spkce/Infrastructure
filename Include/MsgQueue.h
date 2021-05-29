#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include <string>
#include <mqueue.h>

namespace Infra
{

class CMsgQueue
{
public:
	CMsgQueue(std::string name, int maxMsg, int maxMsgLen);
	virtual ~CMsgQueue();

public:
	bool input(const char *msg, size_t len, int timeout = -1, unsigned int prio = 0);
	bool output(char *msg, size_t len, int timeout = -1, unsigned int *priop = NULL);

private:
	mqd_t m_mqId;
	std::string m_name;
};

} //Infra

#endif //__MSG_QUEUE_H__
