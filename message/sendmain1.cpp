#include "msg.h"

int main(int argc, char * argv[]){
	Cmsg msg; 
	msg.SetPath("/tmp/setpath");
	msg.CreateMessageP(0x22);
#if 0
	sleep(5);
	msg.DeleteMessage();
	exit(-1);
#endif
	string s1("hello this is sendmain1 to recvmian1");
	msg.SetMessage(s1, TYPE1);
	msg.SendMessage();
	//msg.ShowMessagInfo();
	//msg.TestForSet();
	sleep(5);
	string s2("hello this is sendmain1 to recvmian2");
	msg.SetMessage(s2, TYPE2);
	msg.SendMessage();
	sleep(5);
	string s3("hello this is sendmain1 to recvmian3");
	//type = TYPE1;
	msg.SetMessage(s3, TYPE3);
	msg.SendMessage();
	sleep(5);
	
	string s4("hello this is sendmain1 to recvmian4");
	//type = TYPE1;
	msg.SetMessage(s4, TYPE4);
	msg.SendMessage();
	sleep(5);
	
	//msg.ShowMessagInfo();
		
	string s5("hello this is sendmain1 to recvmian5");
	//type = TYPE1;
	msg.SetMessage(s5, TYPE5);
	msg.SendMessage();
	sleep(5);
	
	string s6("hello this is sendmain1 to recvmian6");
	msg.SetMessage(s6, TYPE6);
	msg.SendMessage();
	sleep(5);
	//msg.ShowMessagInfo();
		
	string s7("hello this is sendmain1 to recvmian7");
	//type = TYPE1;
	msg.SetMessage(s7, TYPE7);
	msg.SendMessage();
	sleep(5);
		
	return 0;
}
