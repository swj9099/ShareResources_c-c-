#ifndef _USOCK_H
#define _USOCK_H
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <list>
using namespace std;

#define PATH "/tmp/usock"
#define QLEN 10
#define MAXLEN 255

class CUsock {
public:
	CUsock()
		:m_sockfd(0), m_addrlen(0){}
	virtual ~CUsock();
	bool CreateSock();
	void CSetPath(string path);
	bool CBind();
	void CListen();
	void CSetAddr();
	int CGetsockfd();
	
	bool Csend(int sockfd,const char * buf, int len);
	bool Csend(int sockfd, const string buf);
	bool Crecv(int sockfd, string & buf);
	bool Crecv(int sockfd, char * buf);
	void CSetSigPip();
	
	struct sockaddr_un m_addr;
	socklen_t m_addrlen;
protected:
	int CheckFile();

private:
	int m_sockfd;
	string m_path;
	char m_cache[MAXLEN];
};
#endif
