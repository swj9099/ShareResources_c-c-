#include "msg.h"


bool Cmsg::CreateMessageI(){
    ID = msgget((key_t) KEYMESSAGEIPC,0666 | IPC_CREAT);
    if(ID < 0){
		cout << "msgget is failed !!" << endl;
		perror("msgget is failed :");
		return false;
	}
	return true;
}

bool Cmsg::CreateMessageP(int proj_id){
	CreateKeyP();
	m_key = ftok(m_path.c_str(), proj_id);
	if(-1 == m_key){
		cout << "ftok is failed !!"<< endl;
		perror("ftok is failed :");		
		return false;
	}
	
    ID = msgget(m_key,0666 | IPC_CREAT);
    if(ID < 0){
		cout << "msgget is failed !!" << endl;
		perror("msgget is failed :");
		return false;
	}
	
	return true;
}


void Cmsg::SetPath(const char * pathname){
	m_path.assign(pathname);
}
bool Cmsg::Checkfile(){
	return access(m_path.c_str(), F_OK) ? false : true;
}

//暂时不考虑目录不存在情况，目前默认/tmp下家里文件
void Cmsg::CreateKeyP(){
	if(!Checkfile()){
		int ret = 0;
		if((ret = creat(m_path.c_str(), S_IRWXU)) > 0){
			cout << "creat file " << m_path << "success !!"<< endl;
			close(ret);
		}
		else{
			cout << "creat file " << m_path << "failed !!"<< endl;
		}
	}
	
}

bool Cmsg::SetMessage(string str, msgtype type){
	m_sendmybuf.mtype = type;
	bzero(m_sendmybuf.mtext,MAXTXTSIZE);
	strcpy(m_sendmybuf.mtext,str.c_str());
}

string Cmsg::GetMessage(){
	string ss(m_recvmybuf.mtext);
	return ss;
}

void Cmsg::GetMessage(char* rmessg){
	strncpy(rmessg, m_recvmybuf.mtext, MAXTXTSIZE);
}

bool Cmsg::SendMessage(){
	int ret = true;
	ret = msgsnd(ID, &m_sendmybuf, MAXTXTSIZE, IPC_NOWAIT);
	if(0 == ret){
		cout << "msgsnd is success!!!" << endl;
		return true;
	}else{
		perror("msgsnd is failed :");
		return false;
	}
}

bool Cmsg::RecvMessage(msgtype type){
	ssize_t ret = 0;
	ret = msgrcv(ID, &m_recvmybuf,MAXTXTSIZE,type,MSG_NOERROR);
	if(ret > 0){
		cout << "msgrcv is success!!!" << endl;
		return true;
	}else{
		perror("msgrcv is failed :");
		return false;
	}
	
}

void Cmsg::DeleteMessage(){
	//msqid_ds buf;
	if(0 == msgctl(ID, IPC_RMID,NULL))
		cout<< "delete m_key mq is success!!" << hex << m_key << endl;
	else
		perror("delete  mq is failed: ");
}

bool Cmsg::GetMessageInfo(){
	int ret = 0;
	ret = msgctl(ID, IPC_STAT, &m_mqinfo);
	if(0 == ret){
		cout<< "msgctl get mqinfo is success!!" << endl;
		return true;
	}else{
		perror("msgctl get mqinfo is failed :");
		return false;
	}
}

void Cmsg::ShowMessagInfo(){
	if(!GetMessageInfo()){return;}
	cout << "information of message queue id:" << ID << endl;
	cout << "owner's effective user id: " << m_mqinfo.msg_perm.uid << endl;
	cout << "owner's effective group id:" << m_mqinfo.msg_perm.gid << endl;
	cout << "creator's effective user id:" << m_mqinfo.msg_perm.cuid << endl;
	cout << "creator's effective group id:" << m_mqinfo.msg_perm.cgid << endl;
	cout << "access modes:" << m_mqinfo.msg_perm.mode << endl;
	cout << "# of messages on queue: " << m_mqinfo.msg_qnum << endl;
	cout << "max # of bytes on queue: " << m_mqinfo.msg_qbytes << endl;
	cout << "pid of last msgsnd: " << m_mqinfo.msg_lspid << endl;
	cout << "pid of last msgrcv: " << m_mqinfo.msg_lrpid << endl;
	cout << "last-msgsnd time: " << ctime(&(m_mqinfo.msg_stime)) << endl;
	cout << "last-msgrcv time: " << ctime(&(m_mqinfo.msg_rtime)) << endl;
	cout << "last-change time: " << ctime(&(m_mqinfo.msg_ctime)) << endl;
	
}

bool Cmsg::SetMessageInfo(msqid_ds m_mqinfo){
	int ret = 0;
	ret = msgctl(ID, IPC_SET, &m_mqinfo);
	if(0 == ret){
		cout << "msgctl set mqinfo is success" << endl;
		return true;
	}else{
		perror("msgctl set mqinfo is failed");
		return false;
	}
}

//for test IPC_SET
void Cmsg::TestForSet(){
	ShowMessagInfo();
	m_mqinfo.msg_perm.uid = 66;
	SetMessageInfo(m_mqinfo);
	ShowMessagInfo();
	m_mqinfo.msg_perm.uid = 0;
	ShowMessagInfo();
}

