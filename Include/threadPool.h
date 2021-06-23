#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

namespace Infra
{

class CThreadCore;

class CPoolThread
{
public:
	typedef TFuncation1<void, void *> ThreadProc_t;
public:
	CPoolThread();
	virtual ~CPoolThread();

	bool run(bool isBlock);
	bool stop(bool isBlock);
	bool attach(ThreadProc_t proc);
	bool detach(ThreadProc_t proc);
private:
	CThreadCore* m_threadCore;
	ThreadProc_t m_proc;
}

} //Infra

#endif //__THREAD_POOL_H__