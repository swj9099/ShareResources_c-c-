#include "Usock.h"

CUsock::~CUsock(){

}

bool CUsock::CreateSock(){
	m_sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(m_sockfd > 0){
		cout << "CreateSock is success" << endl;
		return true;
	}else{
		perror("CreateSock is failed: ");
		return false;
	}

}

void CUsock::CSetPath(string path){
	m_path = path;
}

int CUsock::CGetsockfd(){
	return m_sockfd;
}

int CUsock::CheckFile(){
	
	if(m_path.empty()){
		cout << "path is not set" << endl;
		return -1;
	}
	if(0 == access(m_path.c_str(),F_OK)){
		unlink(m_path.c_str());
		cout << m_path << "file is exist , unlink" << endl;
	}
	return 0;
}

void CUsock::CSetAddr(){
	bzero(&m_addr,sizeof(m_addr));
	m_addr.sun_family = AF_UNIX;
	//strncpy(m_addr.sun_path, m_path.c_str(), sizeof(m_addr.sun_path) - 1);
	strcpy(m_addr.sun_path, PATH);
	m_addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(PATH);
}

bool CUsock::CBind(){
	int ret = 0;
	CheckFile();
	if(bind(m_sockfd, (struct sockaddr *)&m_addr,m_addrlen) < 0){
		perror("bind is failed: ");
	}
	
}

void CUsock::CListen(){
	if(listen(m_sockfd,QLEN) < 0){
		perror("CListen is failed: ");
	}
}




bool CUsock::Csend(int sockfd,const char * buf, int len){
	ssize_t retlen = 0;
	if(sockfd < 0 || NULL ==  buf || len <= 0){
		cout << "Csend check(char) is error" << endl;
		return false;
	}
	retlen = write(sockfd, buf, len);	
	if(retlen < 0){
		perror("failed write: ");
		return false;
	}
	
	cout << "send msg length is " << retlen << endl;
	return true;
}

bool CUsock::Csend(int sockfd, const string buf){
	ssize_t retlen = 0;
	if(sockfd < 0 || buf.empty()){
		cout << "Csend check(string) is error" << endl;
		return false;
	}
	retlen = write(sockfd, buf.c_str(), buf.length());
	if(retlen < 0){
		perror("failed write: ");
		return false;
	}
	
	cout << "send msg length is " << retlen << endl;

	return true;
}

void CUsock::CSetSigPip(){
	signal(SIGPIPE, SIG_IGN);
}

bool CUsock::Crecv(int sockfd, string & buf){
	ssize_t rcvlen = 0;
	if(sockfd < 0){
		cout << "Crecv(string) sockfd < 0 " << endl;
		return false;
	}
	char tmpbuf[MAXLEN] = {0};
	while((rcvlen = read(sockfd, tmpbuf, MAXLEN)) <= 0){
		if(EINTR == errno)
			continue;
		else{
			perror("read failed is :");
			return false;
		}
	}
	cout << "rcvlen " << rcvlen << endl;
	sleep(2);
	buf.assign(tmpbuf);
	return true;
}

bool CUsock::Crecv(int sockfd, char * buf){
	ssize_t rcvlen = 0;	
	if(sockfd < 0){
		cout << "Crecv(char) sockfd < 0 " << endl;
		return false;
	}
	
	char tmpbuf[MAXLEN] = {0};
	while((rcvlen = read(sockfd, tmpbuf, MAXLEN)) <= 0){
		if(EINTR == errno)
			continue;
		else{
			perror("read failed is :");
			return false;
		}
	}
	if(rcvlen < MAXLEN){
		strcpy(buf, tmpbuf);
	}else{
		strncpy(buf, tmpbuf, MAXLEN -1);
		buf[MAXLEN -1] = 0;
	}
	return true;
}


