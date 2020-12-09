#include "Uclient.h"

bool CUclient::Connect(){
	if(CGetsockfd() < 0 ){
		cout << "Connect check is error : m_sockfd < 0" << endl;
		return false;
	}
	int count = 0;
	while(connect(CGetsockfd(), (struct sockaddr *)&m_addr, m_addrlen) < 0){
		perror("failed to connect: ");
		sleep(1);
		if(3 == count++)
			return false;
	}
	return true;
}
