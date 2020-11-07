#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <vector>
#include <string>
#include <list>
#include <pthread.h>
#include "CPthread.h"
using namespace std;


class CTask{

protected:
	string m_strTaskName;								//任务的名称
	void* m_ptrData;									//要执行任务的具体数据
	
public:
	CTask() = default;
	CTask(string &taskname)
		:m_strTaskName(taskname), m_ptrData(NULL) {}
	virtual int Run() = 0;
	void setData(void* data);							//设置任务数据

	virtual ~CTask() {}

};
CTask* GetTask();

class CThreadPool{

private:
	//static list<CTask*> m_vecTaskCache;						//任务列表
	list<CMyPthread *> m_ThreadList;					//线程存储容器
	static bool stopall;								//线程退出标志
	static bool shutdown;
	static int m_iThreadNum;							//线程池中线程数目
	int m_maxThreadNum;									//申请线程上限
	static volatile int m_idlethread;					//等待任务线程数目

	static pthread_mutex_t m_pthreadMutex;				//线程同步锁
	static pthread_cond_t m_pthreadCond;				//线程同步条件变量

protected:
	static void* ThreadFunc(void *threadData);			//新线程的线程回调函数
	static int MoveToIdle(pthread_t tid);				//线程执行结束后，回归池
	static int MoveToBusy(pthread_t tid);				//线程被调用后，调出池
	int Creat();										//创建线程池中的线程
	void IdleThreadAdd();
	void IdleThreadSub();
	

public:
	CThreadPool();
	CThreadPool(int threadNum);
	int AddTask(CTask *task);							//将任务添加到任务队列中
	int StopAll();										//使线程中所有线程退出
	int GetTaskSize();									//获取当前任务队列中的任务数
	int GetCpuNumber();									//获取当前系统cpu核数
	int GetPoolNum();									//获取当前池线程数目
	//CTask* GetTask();									//从任务队列中获取任务
	void MonitorTask();									//监控任务队列
	
};






#endif
