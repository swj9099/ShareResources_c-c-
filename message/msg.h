#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>


#define KEYMESSAGEIPC 1638
#define MAXTXTSIZE 512
using namespace std;

typedef enum {
    TYPE1 = 1,
    TYPE2,
    TYPE3,
    TYPE4,
    TYPE5,
    TYPE6,
    TYPE7
}msgtype;

typedef struct {
	long mtype;
	char mtext[MAXTXTSIZE];
}Msg_buf;

class Cmsg{
public:
	Cmsg()
		:ID(0), m_path("/tmp/Message"){}
	
	Cmsg(const char *pname)
		:ID(0), m_path(pname){}
	bool CreateMessageI();	
	bool CreateMessageP(int proj_id = 0x66);
	void SetPath(const char * pathname);
	void Setkey(const char * pathname, int proj_id);
	key_t Getkey();
	bool SendMessage();
	bool SetMessage(string str, msgtype type);
	string GetMessage();
	void GetMessage(char* rmessg);
	bool RecvMessage(msgtype type);
	void DeleteMessage();
	void ShowMessagInfo();
	void TestForSet();

protected:
	void CreateKeyP();
	bool Checkfile();
	bool GetMessageInfo();
	bool SetMessageInfo(msqid_ds m_mqinfo);

private:
    int ID;
	string m_path;
	Msg_buf m_sendmybuf;
	Msg_buf m_recvmybuf;
	key_t m_key;
	msqid_ds m_mqinfo;
};

	






