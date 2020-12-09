#ifndef _USERVER_H
#define _USERVER_H
#include "Usock.h"
#include <pthread.h>

void* handle(void* args);

typedef void* (*func)(void *);

typedef struct{
	int sock;
	struct sockaddr_un addr;
	socklen_t addrlen;
	bool used;
}Clientinfo;


class CUserver : public CUsock{
public:
	int CAccept();
	void CHandle(func    handdlefun, void *args);
	friend void* handle(void * args);
protected:
	
private:
	list<Clientinfo*> m_clientinfo;
	
};
#endif
