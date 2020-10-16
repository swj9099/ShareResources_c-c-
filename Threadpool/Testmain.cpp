#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include "CThreadPool.h"

class CMyTask : public CTask {
public:
	CMyTask() = default;
	int Run(){
		printf("%s \n", (char*)m_ptrData);
		int x = rand() % 4 + 1;
		sleep(x);
		return 0;
	}
	~CMyTask() {}

};

int main()
{
	CMyTask taskObj;
	char szTmp[] = "hello!";
	taskObj.setData((void*)szTmp);
	//CThreadPool threadpool(5);
	CThreadPool threadpool;
	printf("threadpool number is %d\n",threadpool.GetPoolNum());

	for(int i = 0; i < 10; i++){
		threadpool.AddTask(&taskObj);
	}

	while(1){
		printf("there are still %d tasks need to handle\n", threadpool.GetTaskSize());
		//任务队列已经没有任务了
		if(threadpool.GetTaskSize() == 0){
			//清除线程池
			if(threadpool.StopAll() == -1){
				printf("thread pool clear, exit. \n");
				exit(0);
			}
				

		}

		sleep(2);
		printf("2 seconds later ...\n");
			
	}
	return 0;
}
