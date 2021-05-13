#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

namespace Infra
{

class IThread;
struct ThreadPoolInternal;
class CThreadPoolManager
{
public:
	static CThreadPoolManager *instance();

private:
	CThreadPoolManager();
	~CThreadPoolManager();
	//bool releaseThread(IThread *pthread);

public:
	IThread *applyThread();

	bool cancelThread(IThread *pthread);

private:
	void workProc(void *arg);

private:
	void allocThread();

	void releaseThread(IThread* pThread);

	struct ThreadPoolInternal* m_pInternal;
};

} //Infra

#endif //__THREAD_POOL_H__