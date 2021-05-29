#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "ctime.h"
#include <string>
#include "LogInternal.h"

namespace Infra
{

class CQueue
{
public:
	CQueue(std::string name, int maxMsg, int maxMsgLen);
	virtual ~CQueue();
public:
	bool input(const char *msg, size_t len, int timeout = -1, unsigned int prio = 0);
	bool output(char *msg, size_t len, int timeout = -1, unsigned int prio = 0);
private:
	mqd_t m_mqId;
	std::string m_name;
};

CQueue::CQueue(std::string name, int maxMsg, int maxMsgLen)
:m_mqId(-1)
,m_name("/")
{
	struct mq_attr attr = {0};
	attr.mq_msgsize = maxMsgLen;
	attr.mq_maxmsg = maxMsg;

	m_name += name;
	InfraTrace("create queue: %s \n", m_name.c_str());

	mq_unlink(m_name.c_str());
	m_mqId = mq_open(m_name.c_str(), O_CREAT|O_RDWR|O_EXCL, 0664, &attr);
	if (m_mqId < 0)
	{
		InfraTrace("create queue: %s fail\n", m_name.c_str());
	}
}

CQueue::~CQueue()
{
	mq_close(m_mqId);
}

bool CQueue::input(const char *msg, size_t len, int timeout, unsigned int prio)
{
	if (msg == NULL || m_mqId < 0)
	{
		InfraTrace("Queue %s not ready\n", m_name.c_str());
		return false;
	}
	
	if (timeout < 0)
	{
		return mq_send(m_mqId, msg, len, prio) == 0;
	}
	else
	{
		struct timespec absTime = {0};
		CTime::covertRealTime((unsigned int)timeout, &absTime);
		return mq_timedsend(m_mqId, msg, len, prio, &absTime) == 0;
	}
}

bool CQueue::output(char *msg, size_t len, int timeout, unsigned int prio)
{
	if (msg == NULL || m_mqId < 0)
	{
		InfraTrace("Queue %s not ready\n", m_name.c_str());
		return false;
	}

	if (timeout < 0)
	{
		return mq_receive(m_mqId, msg, len, &prio) == 0;
	}
	else
	{
		struct timespec absTime = {0};
		CTime::covertRealTime((unsigned int)timeout, &absTime);
		return mq_timedreceive(m_mqId, msg, len, &prio, &absTime) == 0;
	}
}


} //Infra
