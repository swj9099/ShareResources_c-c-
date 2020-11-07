#include <cstdio>
#include <unistd.h>
#include "CThreadPool.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#define _GNU_SOURCE 		/* See feature_test_macros(7) */

#define gettid() syscall(SYS_gettid)

void CTask::setData(void * data){
	m_ptrData = data;
}

//静态成员初始化
pthread_mutex_t m_taskMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_cacheMutex = PTHREAD_MUTEX_INITIALIZER;

vector<CTask*> m_vecTaskList;

bool CThreadPool::stopall = false;
bool CThreadPool::shutdown = false;
int CThreadPool::m_iThreadNum = 0;
list<CTask*> m_vecTaskCache;
volatile int CThreadPool::m_idlethread = 0;
pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;






//线程管理类构造函数
CThreadPool::CThreadPool(){
	m_iThreadNum = GetCpuNumber();
	printf("creat thread number is %d \n",m_iThreadNum);
	Creat();
}

CThreadPool::CThreadPool(int threadNum) {
	m_iThreadNum = threadNum;
	Creat();
}
	

void* CThreadPool::ThreadFunc(void *threadData){

	CMyPthread * mpthread = (CMyPthread *)threadData;
	while(1){
		
		if(stopall){
			mpthread->CStop();
			printf("now i will sleep!!!");
			sleep(1);
		}
		else{
				pthread_mutex_lock(&m_pthreadMutex);
				m_idlethread++;
				pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);	
				pthread_mutex_unlock(&m_pthreadMutex);

				printf("[tid %lu] prepare for run \n",mpthread->CGetThreadId());
				printf("pid is %d \n", gettid());
				printf("pthreadid is % lu \n", pthread_self());
				CTask* task = GetTask();
			
				if(task)
					task->Run();
	
				printf("[tid %lu] \t idle \n",mpthread->CGetThreadId());
		}
		
	}
	
	m_iThreadNum--;
	printf("[tid %lu]\t exit \n",mpthread->CGetThreadId());
	pthread_exit(NULL);
//	return (void*) 0;
}

//向任务队列添加任务

int CThreadPool::AddTask(CTask *task){
	pthread_mutex_lock(&m_taskMutex);
	m_vecTaskList.push_back(task);
	pthread_mutex_unlock(&m_taskMutex);	

	return 0;
}

//创建线程

int CThreadPool::Creat(){
	for(int i =0; i < m_iThreadNum; i++){
		m_ThreadList.push_back(new(CMyPthread));
	}

	for(list<CMyPthread *>::iterator iter = m_ThreadList.begin();iter != m_ThreadList.end(); iter++){
		(*iter)->CSetSeparate();
		(*iter)->CCreate(ThreadFunc, (void *)(*iter), true);
	}
	//m_idlethread = m_iThreadNum;
	return 0;
}

//停止所有线程
int CThreadPool::StopAll(){
	if(stopall){
		return -1;
	}
	printf("Now i will stop all thread!!!\n");

	stopall = true;
	pthread_cond_broadcast(&m_pthreadCond);

	if(0 == m_iThreadNum){
		printf("all thread is stop!!!\n");
	}
		
	//清理僵尸进程
//	for(int i = 0; i< m_iThreadNum; i++){
//		pthread_join(pthread_id[i], NULL);
//	}

//	delete[] pthread_id;
//	pthread_id = NULL;

	//销毁互斥锁和条件变量
//	pthread_mutex_destroy(&m_pthreadMutex);
//	pthread_cond_destroy(&m_pthreadCond);
//	pthread_mutex_destroy(&m_taskMutex);
	return 0;
}

int CThreadPool::GetTaskSize(){
	return m_vecTaskList.size();
}

int CThreadPool::GetCpuNumber(){
	int iProcessors;
    iProcessors = sysconf(_SC_NPROCESSORS_CONF);
    return iProcessors;
}

int CThreadPool::GetPoolNum(){
	return m_iThreadNum;
}



void CThreadPool::MonitorTask(){
	while(1){
		
		if(!stopall && m_idlethread > 0 && !m_vecTaskList.empty()){	
			printf("m_idlethread is %d \n",m_idlethread);
			printf("m_vecTaskList size is %d \n",m_vecTaskList.size());
			vector<CTask*>::iterator iter = m_vecTaskList.begin();
			CTask* task = *iter;
			
			pthread_mutex_lock(&m_taskMutex);
			m_vecTaskList.erase(iter);
			pthread_mutex_unlock(&m_taskMutex);
			
			pthread_mutex_lock(&m_cacheMutex);
			m_vecTaskCache.push_back(task);
			pthread_mutex_unlock(&m_cacheMutex);
			pthread_mutex_lock(&m_pthreadMutex);
			m_idlethread--;
			pthread_mutex_unlock(&m_pthreadMutex);
			pthread_cond_signal(&m_pthreadCond);

		}
		else{
			usleep(2000);
		}
	}
}

void CThreadPool::IdleThreadAdd(){
	m_idlethread++;

}

void CThreadPool::IdleThreadSub(){
	m_idlethread--;
}

CTask* GetTask(){
	list<CTask*>::iterator iter = m_vecTaskCache.begin();

	CTask* task = *iter;
	if(iter != m_vecTaskCache.end()){
		task = *iter;
		pthread_mutex_lock(&m_cacheMutex);
		m_vecTaskCache.erase(iter);
		pthread_mutex_unlock(&m_cacheMutex);
		printf("m_vecTaskCache size is %d \n",m_vecTaskCache.size());
	}
	else{
		printf("sorry there is no task!!!\n");
		return NULL;
	}
	return task;
}


