#ifndef _CPTHREAD_H
#define _CPTHREAD_H
#include <cstdio>
#include <pthread.h>

using namespace std;
class CMyPthread{

public:
	CMyPthread()
		:m_tid(0), start(true), m_attr(NULL) {}
	~CMyPthread();
	
	pthread_t CGetThreadId(){
		return m_tid;
	}
	bool CInitAttr();
	static bool CInitPthreadLock();
	static bool CDestroyThreadLock();
	void CLock();
	void CUnlock();
	static void CExitInThread();
	static void CKillthread(pthread_t tid);
	bool CCreate(void* (*func)(void*), void* arg, bool IsAttr = false);
	static void CSetPthreadName(char* name);
	void CDeleteAttr();
	void CSetSeparate();
	void CSetStackSize(size_t size);
	long CGetMaxNumber(void);	
	void CKillThreaSelf();
	void CCheckCancle();
	void CStop();
	void CStart();
	static pthread_mutex_t g_mutex;

protected:
	pthread_attr_t*  CGetAttr();
private:
	static long m_maxthreadnum;
	bool start;
	pthread_t m_tid;
	pthread_attr_t *m_attr;
	char * data;

};





#endif
