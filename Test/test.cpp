#include <stdio.h>


extern void Log_test(void);
extern void thread_test(void);
extern void threadPool_test(void);
extern void link_test(void);
extern void callback_test(void);
extern void timer_test(void);
extern void MsgQueue_test(void);

int main()
{
	//printf("***************packet_test*************\n");
	//packet_test();
	//printf("****************Log_test***************\n");
	//Log_test();
	//printf("****************thread_test***************\n");
	//thread_test();
	printf("****************threadPool_test***************\n");
	threadPool_test();
	//printf("****************link_test***************\n");
	//link_test();
	//printf("**************callback_test***************\n");
	//callback_test();
	//printf("**************timer_test***************\n");
	//timer_test();
	//printf("**************MsgQueue_test***************\n");
	//MsgQueue_test();

	return 0;
}

//int PointToSegmentDistance(const CCoord & A, const & CCoord & B, const CCoord & C)
//{
//	int cross = (B.x - A.x) * (C.x - A.x) + (B.y - A.y) * (C.y - A.y); //AB·AC = |AB||AC|cosθ
//	if (cross <= 0)
//	{
//		//角BAC是直角或钝角，距离为|AC|
//		return (float)sqrtf((C.x - A.x) * (C.x - A.x) + (C.y - A.y) * (C.y - A.y);
//	}
//		
//
//	int d2 = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y); //|AB||AB|
//	if (cross >= d2)
//	{
//		// AB·AC >= |AB||AB|  角ABC 是直角或钝角 距离为|CB|
//		return (int)sqrt((C.x - B.x) * (C.x - B.x) + (C.y - B.y) * (C.y - B.y);
//	}
//		
//
//	int r = cross / d2;
//	//C在AB上的垂足 （x,y）
//	int x = A.x + (B.x - A.x) * r;
//	int y = A.y + (B.y - A.y) * r;
//
//	return (int)sqrt((C.x - x) * (C.x - x) + (C.y - y) * (C.y - y));
//}


template <typename R, typename P1, typename P2>
class CObServer
{
public:
	typedef Infra::TFuncation2<R, P1, P2> callback_t;

private:
	class CWatcher
	{
	public:
		bool attach(const callback_t);
		bool detach(const callback_t);
		callback_t m_proc;
	}

public:
	bool attach(const callback_t);
	bool detach(const callback_t);

private:
	CWatcher m_watcher;
};