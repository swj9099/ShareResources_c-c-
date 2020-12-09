#include "Uclient.h"

int main(int argc, char *argv[]){
	CUclient client;
	client.CSetSigPip();
	client.CreateSock();
	string path("/tmp/usock");
	client.CSetPath(path);
	client.CSetAddr();
	client.Connect();
	string sendstr;
	while(1){	
    	struct tm* cur_tm;
		time_t now = time(0);
		cur_tm=localtime(&now);
		sendstr.assign(ctime(&now));
		cout <<sendstr << endl;
		if(client.Csend(client.CGetsockfd(),sendstr)){	
			sleep(6);
			continue;
		}else{
			close(client.CGetsockfd());
			break;
		}
		sleep(1);
	}

	return 0;
}

