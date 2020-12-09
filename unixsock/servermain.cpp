#include "Userver.h"

int main(int argc, char *argv[]){
	CUserver server;
	server.CreateSock();
	string path("/tmp/usock");
	server.CSetPath(path);
	server.CSetAddr();
	server.CBind();
	server.CListen();
	if(-1 == server.CAccept()){
		cout << "i will exit" << endl;
		exit(1);
	}
	return 0;
}
