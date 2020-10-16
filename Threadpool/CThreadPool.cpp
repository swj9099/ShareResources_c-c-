#include <cstdio>
#include <unistd.h>
#include "CThreadPool.h"


void CTask::setData(void * data){
	m_ptrData = data;
}

//静态成员初始化

vector<CTask*> CThreadPool::m_vecTaskList;
bool CThreadPool::shutdown = false;
pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

//线程管理类构造函数
CThreadPool::CThreadPool(){
	m_iThreadNum = GetCpuNumber();
	printf("creat thread number is %d \n",m_iThreadNum);
	Creat();
}

CThreadPool::CThreadPool(int threadNum)
	:m_iThreadNum(threadNum) {
	Creat();
}
	

void* CThreadPool::ThreadFunc(void *threadData){
	pthread_t tid = pthread_self(); 
	while(1){
		pthread_mutex_lock(&m_pthreadMutex);
		while(m_vecTaskList.size() == 0 && !shutdown){
			pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
		}

		if(shutdown){
			pthread_mutex_unlock(&m_pthreadMutex);
			printf("[tid %lu]\t exit \n",tid);
			pthread_exit(NULL);
		}

		printf("[tid %lu] run \n",tid);

		vector<CTask*>::iterator iter = m_vecTaskList.begin();

		CTask* task = *iter;
		if(iter != m_vecTaskList.end()){
			task = *iter;
			m_vecTaskList.erase(iter);
		}

		pthread_mutex_unlock(&m_pthreadMutex);

		task->Run();

		printf("[tid %lu] \t idle \n");
	}

	return (void*) 0;
}

//向任务队列添加任务

int CThreadPool::AddTask(CTask *task){
	pthread_mutex_lock(&m_pthreadMutex);
	m_vecTaskList.push_back(task);
	pthread_mutex_unlock(&m_pthreadMutex);
	pthread_cond_signal(&m_pthreadCond);

	return 0;
}

//创建线程

int CThreadPool::Creat(){
	pthread_id = new pthread_t[m_iThreadNum];
	for(int i = 0; i < m_iThreadNum; i++){
		pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
	}

	return 0;
}

//停止所有线程
int CThreadPool::StopAll(){
	if(shutdown){
		return -1;
	}
	printf("Now i will stop all thread!!!\n");

	shutdown = true;
	pthread_cond_broadcast(&m_pthreadCond);

	//清理僵尸进程
	for(int i = 0; i< m_iThreadNum; i++){
		pthread_join(pthread_id[i], NULL);
	}

	delete[] pthread_id;
	pthread_id = NULL;

	//销毁互斥锁和条件变量
	pthread_mutex_destroy(&m_pthreadMutex);
	pthread_cond_destroy(&m_pthreadCond);

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

