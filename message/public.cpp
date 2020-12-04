#include "public.h" 


int CreateMessageI(){

    int ID = 0;
    ID = msgget((key_t) KEYMESSAGEIPC,0666 | IPC_CREAT);
    if(ID > 0)
    	return ID;
    else{
		printf("msgget is failed !!\n");	
		perror("msgget is failed :");
	}

    return -1;
}

int SentMessage(string str, msgtype type){
	Msg_buf mybuf;
	int ret = 0;
	mybuf.mtype = type;
	strcpy(mybuf.mtext,str.c_str());
	
	ret = msgsnd(ID, &mybuf, MAXTXTSIZE, IPC_NOWAIT);
	if(0 == ret){
		printf("msgsnd is success!!!\n");
	}else{
		perror("msgsnd is failed :");
	}
	return ret;
}

int RecvMessage(msgtype type, string &str){
	Msg_buf mybuf;
	ssize_t ret = 0;
	ret = msgrcv(ID, &mybuf,MAXTXTSIZE,type,MSG_NOERROR);
	if(ret > 0){
		printf("msgrcv is success!!!\n");
		string tmpstr(mybuf.mtext);
		str = tmpstr;
	}else{
		perror("msgrcv is failed :");
	}

	return ret;
	
}

