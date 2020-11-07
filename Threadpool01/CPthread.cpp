#include <sys/prctl.h>
#include <unistd.h>
#include "CPthread.h"


long CMyPthread::m_maxthreadnum = sysconf(_SC_THREAD_THREADS_MAX);
pthread_mutex_t CMyPthread::g_mutex = PTHREAD_MUTEX_INITIALIZER;

//PTHREAD_MUTEX_INITIALIZER;


CMyPthread::~CMyPthread(){
	if(m_attr){
		delete m_attr;
		m_attr = NULL;
	}
}

long CMyPthread::CGetMaxNumber(void){
	return m_maxthreadnum;
}

bool CMyPthread::CInitPthreadLock(){
	if(pthread_mutex_init(&g_mutex, NULL)){
		printf("InitPthreadLock failed!!! \n");
		return false;
	}

	return true;
}

bool CMyPthread::CDestroyThreadLock(){
	pthread_mutex_destroy(&g_mutex);
}


void CMyPthread::CLock(){
	pthread_mutex_lock(&g_mutex);
}

void CMyPthread::CUnlock(){
	pthread_mutex_unlock(&g_mutex);
}


bool CMyPthread::CCreate(void* (*func)(void*), void *arg, bool IsAttr){
	int ret = -1;
	
	if(IsAttr)
		ret = pthread_create(&m_tid, NULL, func, arg);
	else
		ret = pthread_create(&m_tid, m_attr, func, arg);
	
	return (0 == ret)? true : false;
}

void CMyPthread::CSetPthreadName(char* name){	
	if(!prctl(PR_SET_NAME,name))
		printf("SetPthreadName is failed! \n");
}

bool CMyPthread::CInitAttr(){
	int ret = 0;
	m_attr = new(pthread_attr_t);
	if(m_attr){
		printf("new m_attr is failed !!!\n");
		return false;
	}
	if(!(ret = pthread_attr_init(m_attr))){
		printf("pthread_attr_init failed error is %d\n", ret);
	}

	return true;
}

void CMyPthread::CSetSeparate(){
	if(m_attr && pthread_attr_setdetachstate(m_attr, PTHREAD_CREATE_DETACHED)){
		printf("SetSeparate failed! \n");
	}	
}

pthread_attr_t* CMyPthread::CGetAttr(){
	return m_attr;
}

void CMyPthread::CDeleteAttr(){
	int ret = 0;
	if((NULL != m_attr) && (!(ret = pthread_attr_destroy(m_attr)))){
		printf("pthread_attr_destroy failed error is %d\n", ret);
	}
	m_attr = NULL;
}

void CMyPthread::CSetStackSize(size_t size){
	if(m_attr && pthread_attr_setstacksize(m_attr, size)){
		printf("SetStackSize failed! \n");
	}	
}

void CMyPthread::CExitInThread(){
	pthread_exit((void*)0);
}


//以下取消线程机制慎用，此处仅做记录，未测试
//发送取消请求，线程中取消点退出
void CMyPthread::CKillthread(pthread_t tid){
	pthread_cancel(tid);
}
//发送取消请求，线程中取消点退出
void CMyPthread::CKillThreaSelf(){
	pthread_cancel(m_tid);
}

//在程序中添加自己的取消点
//其他取消点可参考UNIX高级环境编程12.7内容
void CMyPthread::CCheckCancle(){
	pthread_testcancel();
}

void CMyPthread::CStop(){
	start = false;
}

void CMyPthread::CStart(){
	start = true;
}




