#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

namespace Infra
{

class CQueue
{
public:
	CQueue(std:string name, int maxMsg, int maxMsgLen);
	virtual ~CQueue();
public:
	bool input(const char *pMsg);
private:
	mqd_t m_mqId;
	std::string m_name;
};

CQueue::CQueue(std:string name, int maxMsg, int maxMsgLen)
:m_name("/")
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

bool CQueue::input(const char *msg, size_t len, timeout, unsigned int prio)
{

}

} //Infra


UINT32 sys_mqueue_send(MSG_Q_ID *mqId, const CHAR *pMsg, UINT32 uLen, INT32 iWaitMs)
{
    INT32 iRet = 0;
    struct timespec absTime = {0};

    if (pMsg == NULL || mqId == NULL)
    {
        SYS_POSIX_ERROR("Mqueue send invalid ptr.\n");
        return ERROR;
    }

    if (iWaitMs == WAIT_FOREVER)
    {
        iRet = mq_send(*mqId, pMsg, uLen, MSG_Q_PRIO);
    }
    else
    {
        /*TODO J6平台设置超时时间在校时时会导致等待时间和预计的不一致*/
        sys_timespec_get(iWaitMs , &absTime);
        iRet = mq_timedsend(*mqId, pMsg, uLen, MSG_Q_PRIO, &absTime);
    }

    return iRet;
}
/**@brief  接受消息
 * @param  mqId     消息队列句柄
 * @param  pMsg     接受消息缓存
 * @param  uLen     缓存大小
 * @param  iWaitMs  阻塞时长
 * @return 成功返回 接收的字节数
 * @return 错误返回 ERROR
 * @see
 * @note
 */
UINT32 sys_mqueue_recv(MSG_Q_ID *mqId, CHAR *pMsgBuf, INT32 iBufLen, INT32 iWaitMs)
{
    INT32 iRet = 0;
    struct timespec absTime = {0};

    if (pMsgBuf == NULL || mqId == NULL)
    {
        SYS_POSIX_ERROR("Mqueue send invalid ptr.\n");
        return ERROR;
    }

    if (WAIT_FOREVER == iWaitMs)
    {
        iRet = mq_receive(*mqId, pMsgBuf, iBufLen, MSG_Q_PRIO);
    }
    else if(NO_WAIT == iWaitMs)
    {
        memset(&absTime, 0, sizeof(absTime));
        iRet = mq_timedreceive(*mqId, pMsgBuf, iBufLen, MSG_Q_PRIO, &absTime);
    }
    else
    {
        #if 1
        sys_timespec_get(iWaitMs , &absTime);
        iRet = mq_timedreceive(*mqId, pMsgBuf, iBufLen, MSG_Q_PRIO, &absTime);
        #else
        /*J6平台设置超时时间在校时时会导致等待时间和预计的不一致*/
        SYS_POSIX_ERROR("mqueue not support timeout recv, please use NO_WAIT!\n");
        return ERROR;
        #endif
    }

    return iRet;
}