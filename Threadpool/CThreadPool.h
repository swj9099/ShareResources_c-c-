#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <vector>
#include <string>
#include <pthread.h>

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

class CThreadPool{

private:
	static vector<CTask*> m_vecTaskList;				//任务列表
	static bool shutdown;								//线程退出标志
	int m_iThreadNum;									//线程池种启动线程数目
	int m_maxThreadNum;
	pthread_t *pthread_id;

	static pthread_mutex_t m_pthreadMutex;				//线程同步锁
	static pthread_cond_t m_pthreadCond;				//线程同步条件变量

protected:
	static void* ThreadFunc(void *threadData);			//新线程的线程回调函数
	static int MoveToIdle(pthread_t tid);				//线程执行结束后，回归池
	static int MoveToBusy(pthread_t tid);				//线程被调用后，调出池
	int Creat();										//创建线程池中的线程

public:
	CThreadPool();
	CThreadPool(int threadNum);
	int AddTask(CTask *task);							//将任务添加到任务队列中
	int StopAll();										//使线程中所有线程退出
	int GetTaskSize();									//获取当前任务队列中的任务数
	int GetCpuNumber();									//获取当前系统cpu核数
	int GetPoolNum();									//获取当前池线程数目
	
};






#endif
