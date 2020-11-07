#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include "CThreadPool.h"

class CMyTask : public CTask {
public:
	CMyTask() = default;
	int Run(){
		printf("test..... \n");
		int x = rand() % 4 + 1;
		printf("prepare for sleep %d s\n",x);
		sleep(x);
		return 0;
	}
	~CMyTask() {}

};

int main()
{
	CMyTask taskObj;
	CThreadPool threadpool(50);
	printf("threadpool number is %d\n",threadpool.GetPoolNum());

	for(int i = 0; i < 1000; i++){
		threadpool.AddTask(&taskObj);
	}
	
	
	threadpool.MonitorTask();
	return 0;
}
