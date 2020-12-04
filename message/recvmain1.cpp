#include "msg.h"

int main(int argc, char* argv[]){
	
	Cmsg msg; 
	msg.SetPath("/tmp/setpath");
	msg.CreateMessageP(0x22);
	string s1;
	while(msg.RecvMessage(TYPE1) > 0){
		std::cout << "hello this is recvmain1" << std::endl;
		std::cout << msg.GetMessage() << std::endl;	
		//msg.ShowMessagInfo();
	}
	return 0;
}
