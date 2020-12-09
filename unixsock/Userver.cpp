#include "Userver.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* handle(void* args){
	pthread_detach(pthread_self());
	//cout << hfd << "hfd" << endl;
	Clientinfo  *cl = NULL;
	CUserver * p = (CUserver *)args;
	cout << "m_clientinfo size is " << p->m_clientinfo.size();
	list<Clientinfo*>::iterator iter = p->m_clientinfo.begin();
	
	//请注意此处顺序容器erase用法
	for(; iter != p->m_clientinfo.end(); ){
		if(!((*iter)->used)){		
			cl = (*iter);
			pthread_mutex_lock(&mutex1);
			cl->used = true;
			//调用erase（）函数后，vector后面的元素会向前移位，形成新的容器，这样原来指向删除元素的迭代器（_Where）就失效了。
			//在erase后，iter失效，并不是指向list的下一个元素，iter成了一个“野指针”。
			iter = p->m_clientinfo.erase(iter);
			pthread_mutex_unlock(&mutex1);
			break;
		}else
			iter++;
	}
	cout << "cl->sock" << cl->sock << endl;
	if(cl->sock < 0){
		cout << "hfd <0 " << endl;
		return (void*) 0;
	}
	
	string s;
	while(p->Crecv(cl->sock, s)){
		cout << s << endl;
	}
	close(cl->sock);
	delete cl;
	cl = NULL;
	cout << "handle end" << endl;
}

void CUserver::CHandle(func handdlefun, void *args){
	pthread_t thread1;
    pthread_create(&thread1, NULL, handdlefun, args);
}

int CUserver::CAccept(){
	while(1){
		Clientinfo *cl =(Clientinfo *) new Clientinfo;	
		cl->used = false;
		cl->sock = accept(CGetsockfd(),(struct sockaddr*)&cl->addr,&cl->addrlen);
		cout << "cl->sock " << cl->sock << endl;
		char tmp[125] = {0};
		strncpy(tmp, cl->addr.sun_path, cl->addrlen);
		cout << "cl.addr.sun_path " << tmp << endl;
		cout << "cl.addrlen " << cl->addrlen << endl;
		if(cl->sock > 0){
			pthread_mutex_lock(&mutex1);
			m_clientinfo.push_back(cl);
			pthread_mutex_unlock(&mutex1);
			CHandle(handle, this);
		}else if(EINTR == errno){	
		//restart accept
			continue;
		}else{
			perror("CAccept faied :");
			return -1;
		}
	}
}

